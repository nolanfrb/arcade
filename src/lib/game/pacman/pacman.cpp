/*
** EPITECH PROJECT, 2026
** arcade
** File description:
** pacman
*/

#include "pacman.hpp"
#include <cstdint>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>
#include "../../../shared/Entity.hpp"
#include "../../../shared/Input.hpp"
#include "../../../shared/Position.hpp"
#include "../../../shared/Sound.hpp"
#include "../../../shared/interface/IGame.hpp"

namespace {
enum : std::uint8_t {
  WALL_INDEX = 0,
  PACMAN_INDEX = 1,
  RED_GHOST_INDEX = 2,
  BLUE_GHOST_INDEX = 3,
  PINK_GHOST_INDEX = 4,
  ORANGE_GHOST_INDEX = 5,
  SUPERPACGUM_INDEX = 6,
  PACGUM_INDEX = 7,
  FOOD_INDEX = 8,
  CHASED_GHOST_INDEX = 9,
  DEAD_GHOST_INDEX = 10,
  GHOST_DOOR_INDEX = 11,
};

constexpr int SUPERPACGUM_DURATION = 10;
constexpr float GHOST_SPEED_DELAY = 0.2F;

constexpr std::uint8_t PACMAN_FRAME_COUNT = 3;
constexpr std::uint8_t GHOST_DIRECTION_FRAME_COUNT = 2;
constexpr std::uint8_t SCARED_GHOST_FRAME_COUNT = 2;
constexpr std::uint8_t DEAD_GHOST_FRAME_COUNT = 4;

// NOLINTNEXTLINE(bugprone-easily-swappable-parameters)
float getCenteredTextPosX(const std::string& text, int fontSize,
                          float screenWidthPx, float tilePx) {
  const float textWidthPx =
      static_cast<float>(text.size()) * static_cast<float>(fontSize);
  return (screenWidthPx - textWidthPx) / (2.F * tilePx);
}

std::string buildTexturePath(const std::string& folder, std::uint8_t frame) {
  return folder + "/texture_" + std::to_string(frame) + ".png";
}

std::string getPacmanDirectionFolder(Input direction) {
  switch (direction) {
    case Input::UP:
      return "assets/pacman/pacman/up";
    case Input::DOWN:
      return "assets/pacman/pacman/down";
    case Input::LEFT:
      return "assets/pacman/pacman/left";
    case Input::RIGHT:
    case Input::NONE:
    default:
      return "assets/pacman/pacman/right";
  }
}

std::string getGhostColorFolder(const Entity& ghost) {
  if (ghost.type.color == RED) {
    return "assets/pacman/redGhost";
  }
  if (ghost.type.color == BLUE) {
    return "assets/pacman/blueGhost";
  }
  if (ghost.type.color == PINK) {
    return "assets/pacman/pinkGhost";
  }
  if (ghost.type.color == ORANGE) {
    return "assets/pacman/orangeGhost";
  }
  return "assets/pacman/redGhost";
}

std::uint8_t getDirectionBaseFrame(Input direction) {
  constexpr std::uint8_t UP_BASE_FRAME = 4;
  constexpr std::uint8_t RIGHT_BASE_FRAME = 0;
  constexpr std::uint8_t DOWN_BASE_FRAME = 6;
  constexpr std::uint8_t LEFT_BASE_FRAME = 2;

  switch (direction) {
    case Input::UP:
      return UP_BASE_FRAME;
    case Input::RIGHT:
      return RIGHT_BASE_FRAME;
    case Input::DOWN:
      return DOWN_BASE_FRAME;
    case Input::LEFT:
      return LEFT_BASE_FRAME;
    case Input::NONE:
    default:
      return UP_BASE_FRAME;
  }
}
}  // namespace

namespace gsl {
template <typename T>
using owner = T;
}  // namespace gsl

int Pacman::getTypeIndex(type tile) {
  static const std::unordered_map<type, int> typeToIndex = {
      {type::WALL, WALL_INDEX},
      {type::PACMAN, PACMAN_INDEX},
      {type::RED_GHOST, RED_GHOST_INDEX},
      {type::BLUE_GHOST, BLUE_GHOST_INDEX},
      {type::PINK_GHOST, PINK_GHOST_INDEX},
      {type::ORANGE_GHOST, ORANGE_GHOST_INDEX},
      {type::SUPERPACGUM, SUPERPACGUM_INDEX},
      {type::PACGUM, PACGUM_INDEX},
      {type::FOOD, FOOD_INDEX},
      {type::CHASED_GHOST, CHASED_GHOST_INDEX},
      {type::DEAD_GHOST, DEAD_GHOST_INDEX},
      {type::GHOST_DOOR, GHOST_DOOR_INDEX}};

  auto type = typeToIndex.find(tile);
  return (type != typeToIndex.end()) ? type->second : 0;
}

void Pacman::init() {
  setName("Pacman");
  setIsGameOver(false);
  if (loadMap("assets/pacman/pacman_map.txt")) {
    if (checkMap()) {
      createEntitiesType();
      createEntities();
      addSound(Sound{.filePath = "assets/pacman/sounds/game_start.wav"});
    }
  }
}

void Pacman::stop() {
  _map.clear();
  _foods.clear();
  _pacgums.clear();
  _superPacgums.clear();
  _ghosts.clear();
  _deadGhosts.clear();
  _chassedGhosts.clear();
  _ghostDirections.clear();
  _isSuperPacgumActive = false;
  _score = 0;
  _level = 1;
  _ghostSpeedMultiplier = 1;
  _gameTimer = 0;
  _playerMovementTimer = 0;
  clearEntities();
  clearTexts();
}

void Pacman::restart() {
  stop();
  init();
}

void Pacman::checkSuperPacgumTimer(float deltaTime) {
  if (_isSuperPacgumActive) {
    _superPacgumTimer += deltaTime;
    if (_superPacgumTimer >= SUPERPACGUM_DURATION) {
      _isSuperPacgumActive = false;
      _superPacgumTimer = 0;
    }
  }
}

void Pacman::updateEntities(std::vector<Entity>& entities,
                            const std::vector<EntityType>& entityTypes,
                            float deltaTime) {
  (void)deltaTime;
  auto addWithOffset = [&entities](const Entity& src) {
    Entity offsetEntity = src;
    offsetEntity.position = toScreenPos(src.position);
    entities.push_back(offsetEntity);
  };

  for (const auto& item : _foods) {
    addWithOffset(item);
  }
  for (const auto& item : _pacgums) {
    addWithOffset(item);
  }
  for (const auto& item : _superPacgums) {
    addWithOffset(item);
  }
  for (auto ghost : _ghosts) {
    if (_isSuperPacgumActive) {
      ghost.type = entityTypes[getTypeIndex(type::CHASED_GHOST)];
    }
    addWithOffset(ghost);
  }
  for (const auto& ghost : _deadGhosts) {
    Entity deadGhostEntity = ghost;
    deadGhostEntity.type = entityTypes[getTypeIndex(type::DEAD_GHOST)];
    addWithOffset(deadGhostEntity);
  }
  for (const auto& ghost : _chassedGhosts) {
    addWithOffset(ghost);
  }
  addWithOffset(_player);
  setEntities(entities);
}

void Pacman::checkVictory() {
  if (_pacgums.empty() && _superPacgums.empty()) {
    _ghostSpeedMultiplier += LEVEL_UP_SPEED_INCREASE;
    _level++;
    int savedLevel = _level;
    int savedScore = _score;
    float savedSpeed = _ghostSpeedMultiplier;
    restart();
    _level = savedLevel;
    _score = savedScore;
    _ghostSpeedMultiplier = savedSpeed;
    setScore(_score);
  }
}

void Pacman::updateAnimation(float deltatime) {
  _animationTimer += deltatime;
  while (_animationTimer >= GHOST_SPEED_DELAY) {
    _animationTimer -= GHOST_SPEED_DELAY;
    _pacmanAnimationFrame = (_pacmanAnimationFrame + 1) % PACMAN_FRAME_COUNT;
    _ghostAnimationFrame =
        (_ghostAnimationFrame + 1) % GHOST_DIRECTION_FRAME_COUNT;
    _scaredGhostAnimationFrame =
        (_scaredGhostAnimationFrame + 1) % SCARED_GHOST_FRAME_COUNT;
    _deadGhostAnimationFrame =
        (_deadGhostAnimationFrame + 1) % DEAD_GHOST_FRAME_COUNT;
  }

  const Input playerDirection =
      _lastPlayerInput == Input::NONE ? Input::RIGHT : _lastPlayerInput;
  _player.type.spritePath = buildTexturePath(
      getPacmanDirectionFolder(playerDirection), _pacmanAnimationFrame);

  for (std::size_t i = 0; i < _ghosts.size(); ++i) {
    Input ghostDirection = Input::UP;
    if (i < _ghostDirections.size()) {
      ghostDirection = _ghostDirections[i];
    }
    const std::uint8_t frame =
        getDirectionBaseFrame(ghostDirection) + _ghostAnimationFrame;
    _ghosts[i].type.spritePath =
        buildTexturePath(getGhostColorFolder(_ghosts[i]), frame);
  }

  std::vector<EntityType> entityTypes = getEntityTypes();
  if (entityTypes.size() > CHASED_GHOST_INDEX) {
    entityTypes[CHASED_GHOST_INDEX].spritePath = buildTexturePath(
        "assets/pacman/scaredGhost", _scaredGhostAnimationFrame);
  }
  if (entityTypes.size() > DEAD_GHOST_INDEX) {
    entityTypes[DEAD_GHOST_INDEX].spritePath =
        buildTexturePath("assets/pacman/deadGhost", _deadGhostAnimationFrame);
  }
  setEntityTypes(entityTypes);
}

Position Pacman::toScreenPos(const Position& mapPos) {
  return Position{.x = mapPos.x + MAP_OFFSET_X, .y = mapPos.y + MAP_OFFSET_Y};
}

void Pacman::displayHUD() {
  clearTexts();
  std::ostringstream scoreStream;
  scoreStream << "Score: " << _score;

  Text scoreText;
  scoreText.content = scoreStream.str();
  scoreText.position = Position{.x = MAP_OFFSET_X, .y = HUD_Y};
  scoreText.fontSize = 14;
  scoreText.color = WHITE;
  scoreText.fontPath = "";
  addText(scoreText);

  std::ostringstream levelStream;
  levelStream << "Level: " << _level;

  Text levelText;
  levelText.content = levelStream.str();
  levelText.position = Position{
      .x = MAP_OFFSET_X + static_cast<float>(MAP_COLS) - 8.F, .y = HUD_Y};
  levelText.fontSize = 14;
  levelText.color = WHITE;
  levelText.fontPath = "";
  addText(levelText);
}

void Pacman::displayGameOver() {
  constexpr float SCREEN_WIDTH_PX = static_cast<float>(SCREEN_TILES_X) * 20.F;
  constexpr float SCREEN_CENTER_Y = static_cast<float>(SCREEN_TILES_Y) / 2.F;
  constexpr float TILE_PX = 20.F;

  clearTexts();

  std::ostringstream scoreStream;
  scoreStream << "GAME OVER! Final Score: " << _score;

  Text gameOverText;
  gameOverText.content = scoreStream.str();
  gameOverText.fontSize = 16;
  gameOverText.position = Position{
      .x = getCenteredTextPosX(gameOverText.content, gameOverText.fontSize,
                               SCREEN_WIDTH_PX, TILE_PX),
      .y = SCREEN_CENTER_Y - 1.F};
  gameOverText.color = RED;
  gameOverText.fontPath = "";
  addText(gameOverText);

  Text restartText;
  restartText.content = "Press R to Restart or ESC for Menu";
  restartText.fontSize = 14;
  restartText.position = Position{
      .x = getCenteredTextPosX(restartText.content, restartText.fontSize,
                               SCREEN_WIDTH_PX, TILE_PX),
      .y = SCREEN_CENTER_Y + 1.F};
  restartText.color = WHITE;
  restartText.fontPath = "";
  addText(restartText);
}

void Pacman::update(Input input, float deltaTime) {
  if (isGameOver()) {
    displayGameOver();
    return;
  }

  _gameTimer += deltaTime;
  _playerMovementTimer += deltaTime;
  checkCollision();
  checkBordersCollision();
  movePlayer(input);
  moveGhosts(deltaTime, input);
  updateAnimation(deltaTime);
  setScore(_score);
  checkVictory();
  checkSuperPacgumTimer(deltaTime);

  std::vector<Entity> entities;
  const std::vector<EntityType>& entityTypes = getEntityTypes();
  for (int xCoordinate = 0; xCoordinate < _map.size(); xCoordinate++) {
    for (int yCoordinate = 0; yCoordinate < _map[xCoordinate].size();
         yCoordinate++) {
      if (_map[xCoordinate][yCoordinate] == type::WALL) {
        Entity wall;
        wall.type = entityTypes[getTypeIndex(type::WALL)];
        wall.position =
            toScreenPos(Position{.x = static_cast<float>(yCoordinate),
                                 .y = static_cast<float>(xCoordinate)});
        entities.push_back(wall);
      } else if (_map[xCoordinate][yCoordinate] == type::GHOST_DOOR) {
        Entity door;
        door.type = entityTypes[getTypeIndex(type::GHOST_DOOR)];
        door.position =
            toScreenPos(Position{.x = static_cast<float>(yCoordinate),
                                 .y = static_cast<float>(xCoordinate)});
        entities.push_back(door);
      }
    }
  }
  displayHUD();
  updateEntities(entities, entityTypes, deltaTime);
}

extern "C" gsl::owner<IGame*> createGame() { return new Pacman(); }
extern "C" void destroyGame(gsl::owner<IGame*> game) { delete game; }
