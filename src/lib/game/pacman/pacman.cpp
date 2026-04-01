/*
** EPITECH PROJECT, 2026
** arcade
** File description:
** pacman
*/

#include "pacman.hpp"
#include <cstdint>
#include <string>
#include <unordered_map>
#include <vector>
#include "../../../shared/Entity.hpp"
#include "../../../shared/Input.hpp"
#include "../../../shared/Position.hpp"
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
  if (loadMap("assets/pacman/pacman_map.txt")) {
    if (checkMap()) {
      createEntitiesType();
      createEntities();
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
  _ghostSpeedMultiplier = 1.0F;
  _gameTimer = 0;
  _playerMovementTimer = 0;
  _gameTimer = 0;
  _playerMovementTimer = 0;
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
  for (const auto& item : _foods) {
    entities.push_back(item);
  }
  for (const auto& item : _pacgums) {
    entities.push_back(item);
  }
  for (const auto& item : _superPacgums) {
    entities.push_back(item);
  }
  for (auto ghost : _ghosts) {
    if (_isSuperPacgumActive) {
      ghost.type = entityTypes[getTypeIndex(type::CHASED_GHOST)];
    }
    entities.push_back(ghost);
  }
  for (const auto& ghost : _deadGhosts) {
    Entity deadGhostEntity = ghost;
    deadGhostEntity.type = entityTypes[getTypeIndex(type::DEAD_GHOST)];
    entities.push_back(deadGhostEntity);
  }
  for (const auto& ghost : _chassedGhosts) {
    entities.push_back(ghost);
  }
  entities.push_back(_player);
  setEntities(entities);
}

void Pacman::checkVictory() {
  if (_pacgums.empty() && _superPacgums.empty()) {
    _ghostSpeedMultiplier += LEVEL_UP_SPEED_INCREASE;
    restart();
  }
}

void Pacman::update(Input input, float deltaTime) {
  _gameTimer += deltaTime;
  _playerMovementTimer += deltaTime;
  checkCollision();
  checkBordersCollision();
  movePlayer(input);
  moveGhosts(deltaTime, input);
  setScore(_score);
  checkVictory();
  if (isGameOver()) {
    stop();
    return;
  }
  checkSuperPacgumTimer(deltaTime);

  std::vector<Entity> entities;
  const std::vector<EntityType>& entityTypes = getEntityTypes();
  for (int xCoordinate = 0; xCoordinate < _map.size(); xCoordinate++) {
    for (int yCoordinate = 0; yCoordinate < _map[xCoordinate].size();
         yCoordinate++) {
      if (_map[xCoordinate][yCoordinate] == type::WALL) {
        Entity wall;
        wall.type = entityTypes[getTypeIndex(type::WALL)];
        wall.position = Position{.x = static_cast<float>(yCoordinate),
                                 .y = static_cast<float>(xCoordinate)};
        entities.push_back(wall);
      } else if (_map[xCoordinate][yCoordinate] == type::GHOST_DOOR) {
        Entity door;
        door.type = entityTypes[getTypeIndex(type::GHOST_DOOR)];
        door.position = Position{.x = static_cast<float>(yCoordinate),
                                 .y = static_cast<float>(xCoordinate)};
        entities.push_back(door);
      }
    }
  }
  updateEntities(entities, entityTypes, deltaTime);
}

extern "C" gsl::owner<IGame*> createGame() { return new Pacman(); }
