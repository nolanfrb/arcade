/*
** EPITECH PROJECT, 2026
** arcade
** File description:
** pacman
*/

#include "pacman.hpp"
#include <string>
#include "../../../shared/Entity.hpp"

#define WALL_INDEX 0
#define PACMAN_INDEX 1
#define GHOST_INDEX 2
#define SUPERPACGUM_INDEX 3
#define PACGUM_INDEX 4
#define FOOD_INDEX 5
#define CHASSED_GHOST_INDEX 6
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
  int typeIndex = 0;
  switch (tile) {
    case type::WALL:
      typeIndex = WALL_INDEX;
      break;
    case type::PACMAN:
      typeIndex = PACMAN_INDEX;
      break;
    case type::GHOST:
      typeIndex = GHOST_INDEX;
      break;
    case type::SUPERPACGUM:
      typeIndex = SUPERPACGUM_INDEX;
      break;
    case type::PACGUM:
      typeIndex = PACGUM_INDEX;
      break;
    case type::FOOD:
      typeIndex = FOOD_INDEX;
      break;
    case type::CHASSED_GHOST:
      typeIndex = CHASSED_GHOST_INDEX;
      break;
    case type::DEAD_GHOST:
      typeIndex = DEAD_GHOST_INDEX;
      break;
    case type::GHOST_DOOR:
      typeIndex = GHOST_DOOR_INDEX;
      break;
    case type::EMPTY:
      typeIndex = EMPTY_INDEX;
      break;
    default:
      typeIndex = 0;
      break;
  }
  return typeIndex;
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
      ghost.type = entityTypes[getTypeIndex(type::CHASSED_GHOST)];
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
