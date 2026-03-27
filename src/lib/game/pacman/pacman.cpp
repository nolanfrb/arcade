/*
** EPITECH PROJECT, 2026
** arcade
** File description:
** pacman
*/

#include "pacman.hpp"
#include <string>
#include <unordered_map>
#include "../../../shared/Entity.hpp"

#define WALL_INDEX 0
#define PACMAN_INDEX 1
#define GHOST_INDEX 2
#define SUPERPACGUM_INDEX 3
#define PACGUM_INDEX 4
#define FOOD_INDEX 5
#define CHASED_GHOST_INDEX 6
#define DEAD_GHOST_INDEX 7
#define GHOST_DOOR_INDEX 8
#define EMPTY_INDEX 9
#define SUPERPACGUM_DURATION 10
#define GHOST_RESPAWN_DURATION 5
#define GHOST_SPEED_DELAY 0.2F

namespace gsl {
template <typename T>
using owner = T;
}  // namespace gsl

int Pacman::getTypeIndex(type tile) {
  static const std::unordered_map<type, int> typeToIndex = {
      {type::WALL, WALL_INDEX},
      {type::PACMAN, PACMAN_INDEX},
      {type::GHOST, GHOST_INDEX},
      {type::SUPERPACGUM, SUPERPACGUM_INDEX},
      {type::PACGUM, PACGUM_INDEX},
      {type::FOOD, FOOD_INDEX},
      {type::CHASED_GHOST, CHASED_GHOST_INDEX},
      {type::DEAD_GHOST, DEAD_GHOST_INDEX},
      {type::GHOST_DOOR, GHOST_DOOR_INDEX},
      {type::EMPTY, EMPTY_INDEX}};

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
  _ghostDirections.clear();
  _ghostMovementTimer = 0;
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
                            const std::vector<EntityType>& entityTypes) {
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
    entities.push_back(ghost);
  }
  for (const auto& ghost : _chassedGhosts) {
    entities.push_back(ghost);
  }
  entities.push_back(_player);
  setEntities(entities);
}

void Pacman::update(Input input, float deltaTime) {
  checkCollision();
  checkBordersCollision();
  movePlayer(input);
  moveGhosts(deltaTime);
  setScore(_score);
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
        wall.position = Position(static_cast<float>(yCoordinate),
                                 static_cast<float>(xCoordinate));
        entities.push_back(wall);
      } else if (_map[xCoordinate][yCoordinate] == type::GHOST_DOOR) {
        Entity door;
        door.type = entityTypes[getTypeIndex(type::GHOST_DOOR)];
        door.position = Position(static_cast<float>(yCoordinate),
                                 static_cast<float>(xCoordinate));
        entities.push_back(door);
      }
    }
  }
  updateEntities(entities, entityTypes);
}

extern "C" gsl::owner<IGame*> createGame() { return new Pacman(); }
