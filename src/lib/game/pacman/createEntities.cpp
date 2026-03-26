/*
** EPITECH PROJECT, 2026
** arcade
** File description:
** createEntities
*/

#include "pacman.hpp"

#define WHITE {255, 255, 255, 255}
#define BLUE {0, 0, 255, 255}
#define RED {255, 0, 0, 255}

namespace {
EntityType createWallType() {
  EntityType wall = EntityType();
  wall.type = Shape::RECTANGLE;
  wall.width = 1;
  wall.height = 1;
  wall.color = WHITE;
  wall.asciiChar = '#';
  wall.isTextInput = false;
  wall.spritePath = "assets/pacman/wall.png";
  return wall;
}
EntityType createPacmanType() {
  EntityType pacman = EntityType();
  pacman.type = Shape::RECTANGLE;
  pacman.width = 1;
  pacman.height = 1;
  pacman.color = WHITE;
  pacman.asciiChar = 'C';
  pacman.isTextInput = false;
  pacman.spritePath = "assets/pacman/pacman.png";
  return pacman;
}
EntityType createGhostType() {
  EntityType ghost = EntityType();
  ghost.type = Shape::RECTANGLE;
  ghost.width = 1;
  ghost.height = 1;
  ghost.color = WHITE;
  ghost.asciiChar = '^';
  ghost.isTextInput = false;
  ghost.spritePath = "assets/pacman/ghost.png";
  return ghost;
}

EntityType createChassedGhostType() {
  EntityType chassedGhost = EntityType();
  chassedGhost.type = Shape::RECTANGLE;
  chassedGhost.width = 1;
  chassedGhost.height = 1;
  chassedGhost.color = BLUE;
  chassedGhost.asciiChar = 'D';
  chassedGhost.isTextInput = false;
  chassedGhost.spritePath = "assets/pacman/chassedGhost.png";
  return chassedGhost;
}

EntityType createDeadGhostType() {
  EntityType deadGhost = EntityType();
  deadGhost.type = Shape::RECTANGLE;
  deadGhost.width = 1;
  deadGhost.height = 1;
  deadGhost.color = RED;
  deadGhost.asciiChar = 'X';
  deadGhost.isTextInput = false;
  deadGhost.spritePath = "assets/pacman/deadGhost.png";
  return deadGhost;
}

EntityType createSuperPacgumType() {
  EntityType superPacgum = EntityType();
  superPacgum.type = Shape::RECTANGLE;
  superPacgum.width = 1;
  superPacgum.height = 1;
  superPacgum.color = WHITE;
  superPacgum.asciiChar = 'O';
  superPacgum.isTextInput = false;
  superPacgum.spritePath = "assets/pacman/superPacgum.png";
  return superPacgum;
}
EntityType createPacgumType() {
  EntityType pacgum = EntityType();
  pacgum.type = Shape::RECTANGLE;
  pacgum.width = 1;
  pacgum.height = 1;
  pacgum.color = WHITE;
  pacgum.asciiChar = '.';
  pacgum.isTextInput = false;
  pacgum.spritePath = "assets/pacman/pacgum.png";
  return pacgum;
}
EntityType createFoodType() {
  EntityType food = EntityType();
  food.type = Shape::RECTANGLE;
  food.width = 1;
  food.height = 1;
  food.color = WHITE;
  food.asciiChar = 'F';
  food.isTextInput = false;
  food.spritePath = "assets/pacman/food.png";
  return food;
}

EntityType createGhostDoorType() {
  EntityType ghostDoor = EntityType();
  ghostDoor.type = Shape::RECTANGLE;
  ghostDoor.width = 1;
  ghostDoor.height = 1;
  ghostDoor.color = WHITE;
  ghostDoor.asciiChar = '|';
  ghostDoor.isTextInput = false;
  ghostDoor.spritePath = "assets/pacman/ghostDoor.png";
  return ghostDoor;
}
}  // namespace

void Pacman::createEntitiesType() {
  std::vector<EntityType> entityTypes;
  entityTypes.push_back(createWallType());
  entityTypes.push_back(createPacmanType());
  entityTypes.push_back(createGhostType());
  entityTypes.push_back(createSuperPacgumType());
  entityTypes.push_back(createPacgumType());
  entityTypes.push_back(createFoodType());
  entityTypes.push_back(createChassedGhostType());
  entityTypes.push_back(createDeadGhostType());
  entityTypes.push_back(createGhostDoorType());
  setEntityTypes(entityTypes);
}

void Pacman::createEntities() {
  std::vector<Entity> entities;
  const std::vector<EntityType>& entityTypes = getEntityTypes();
  for (int xCoordinate = 0; xCoordinate < _map.size(); xCoordinate++) {
    for (int yCoordinate = 0; yCoordinate < _map[xCoordinate].size();
         yCoordinate++) {
      Entity entity = Entity();
      type tile = getTile(xCoordinate, yCoordinate);
      entity.type = entityTypes[getTypeIndex(tile)];
      entity.position = Position(static_cast<float>(yCoordinate),
                                 static_cast<float>(xCoordinate));
      if (tile == type::PACMAN) {
        _player = entity;
      } else if (tile == type::GHOST) {
        _ghosts.push_back(entity);
        _ghostDirections.push_back(Input::UP);
      } else if (tile == type::SUPERPACGUM) {
        _superPacgums.push_back(entity);
      } else if (tile == type::PACGUM) {
        _pacgums.push_back(entity);
      } else if (tile == type::FOOD) {
        _foods.push_back(entity);
      }
      entities.push_back(entity);
    }
  }
  setEntities(entities);
}
