/*
** EPITECH PROJECT, 2026
** arcade
** File description:
** createEntities
*/

#include <vector>
#include "../../../shared/Entity.hpp"
#include "../../../shared/Input.hpp"
#include "../../../shared/Position.hpp"
#include "pacman.hpp"

namespace {
constexpr float TILE_SIZE = 1.F;
constexpr float GHOST_FRAME_SIZE = 14.F;
constexpr float FRUIT_SIZE = 12.F / GHOST_FRAME_SIZE;
constexpr float SUPERPACGUM_SIZE = 13.F / GHOST_FRAME_SIZE;
constexpr float PACGUM_SIZE = 0.2F;

EntityType createWallType() {
  EntityType wall = EntityType();
  wall.type = Shape::RECTANGLE;
  wall.width = TILE_SIZE;
  wall.height = TILE_SIZE;
  wall.color = WHITE;
  wall.asciiChar = '#';
  wall.isTextInput = false;
  wall.spritePath = "assets/pacman/pacman_border.png";
  return wall;
}
EntityType createPacmanType() {
  EntityType pacman = EntityType();
  pacman.type = Shape::SPRITE;
  pacman.width = TILE_SIZE;
  pacman.height = TILE_SIZE;
  pacman.color = WHITE;
  pacman.asciiChar = 'C';
  pacman.isTextInput = false;
  pacman.spritePath = "assets/pacman/pacman_move_right.png";
  return pacman;
}
EntityType createRedGhostType() {
  EntityType ghost = EntityType();
  ghost.type = Shape::SPRITE;
  ghost.width = TILE_SIZE;
  ghost.height = TILE_SIZE;
  ghost.color = RED;
  ghost.asciiChar = 'R';
  ghost.isTextInput = false;
  ghost.spritePath = "assets/pacman/pacman_red_ghost.png";
  return ghost;
}
EntityType createBlueGhostType() {
  EntityType ghost = EntityType();
  ghost.type = Shape::SPRITE;
  ghost.width = TILE_SIZE;
  ghost.height = TILE_SIZE;
  ghost.color = BLUE;
  ghost.asciiChar = 'B';
  ghost.isTextInput = false;
  ghost.spritePath = "assets/pacman/pacman_blue_ghost.png";
  return ghost;
}
EntityType createPinkGhostType() {
  EntityType ghost = EntityType();
  ghost.type = Shape::SPRITE;
  ghost.width = TILE_SIZE;
  ghost.height = TILE_SIZE;
  ghost.color = PINK;
  ghost.asciiChar = 'P';
  ghost.isTextInput = false;
  ghost.spritePath = "assets/pacman/pacman_pink_ghost.png";
  return ghost;
}
EntityType createOrangeGhostType() {
  EntityType ghost = EntityType();
  ghost.type = Shape::SPRITE;
  ghost.width = TILE_SIZE;
  ghost.height = TILE_SIZE;
  ghost.color = ORANGE;
  ghost.asciiChar = 'N';
  ghost.isTextInput = false;
  ghost.spritePath = "assets/pacman/pacman_orange_ghost.png";
  return ghost;
}

EntityType createChassedGhostType() {
  EntityType chassedGhost = EntityType();
  chassedGhost.type = Shape::SPRITE;
  chassedGhost.width = TILE_SIZE;
  chassedGhost.height = TILE_SIZE;
  chassedGhost.color = BLUE;
  chassedGhost.asciiChar = 'D';
  chassedGhost.isTextInput = false;
  chassedGhost.spritePath = "assets/pacman/pacman_scared_ghost.png";
  return chassedGhost;
}

EntityType createDeadGhostType() {
  EntityType deadGhost = EntityType();
  deadGhost.type = Shape::SPRITE;
  deadGhost.width = TILE_SIZE;
  deadGhost.height = TILE_SIZE;
  deadGhost.color = BLUE;
  deadGhost.asciiChar = 'X';
  deadGhost.isTextInput = false;
  deadGhost.spritePath = "assets/pacman/pacman_dead_ghost.png";
  return deadGhost;
}

EntityType createSuperPacgumType() {
  EntityType superPacgum = EntityType();
  superPacgum.type = Shape::SPRITE;
  superPacgum.width = SUPERPACGUM_SIZE;
  superPacgum.height = SUPERPACGUM_SIZE;
  superPacgum.color = WHITE;
  superPacgum.asciiChar = 'O';
  superPacgum.isTextInput = false;
  superPacgum.spritePath = "assets/pacman/pacman_superpacgum.png";
  return superPacgum;
}
EntityType createPacgumType() {
  EntityType pacgum = EntityType();
  pacgum.type = Shape::CIRCLE;
  pacgum.width = PACGUM_SIZE;
  pacgum.height = PACGUM_SIZE;
  pacgum.color = WHITE;
  pacgum.asciiChar = '.';
  pacgum.isTextInput = false;
  pacgum.spritePath = "assets/pacman/pacman_pacgum.png";
  return pacgum;
}
EntityType createFoodType() {
  EntityType food = EntityType();
  food.type = Shape::SPRITE;
  food.width = FRUIT_SIZE;
  food.height = FRUIT_SIZE;
  food.color = WHITE;
  food.asciiChar = 'F';
  food.isTextInput = false;
  food.spritePath = "assets/pacman/pacman_fruit.png";
  return food;
}

EntityType createGhostDoorType() {
  EntityType ghostDoor = EntityType();
  ghostDoor.type = Shape::RECTANGLE;
  ghostDoor.width = TILE_SIZE;
  ghostDoor.height = TILE_SIZE;
  ghostDoor.color = WHITE;
  ghostDoor.asciiChar = '|';
  ghostDoor.isTextInput = false;
  ghostDoor.spritePath = "";
  return ghostDoor;
}
}  // namespace

void Pacman::createEntitiesType() {
  std::vector<EntityType> entityTypes;
  entityTypes.push_back(createWallType());
  entityTypes.push_back(createPacmanType());
  entityTypes.push_back(createRedGhostType());
  entityTypes.push_back(createBlueGhostType());
  entityTypes.push_back(createPinkGhostType());
  entityTypes.push_back(createOrangeGhostType());
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
      const type tile = getTile(xCoordinate, yCoordinate);
      entity.type = entityTypes[getTypeIndex(tile)];
      entity.position = Position{.x = static_cast<float>(yCoordinate),
                                 .y = static_cast<float>(xCoordinate)};
      if (tile == type::PACMAN) {
        _player = entity;
      } else if (tile == type::RED_GHOST || tile == type::PINK_GHOST ||
                 tile == type::BLUE_GHOST || tile == type::ORANGE_GHOST) {
        _ghosts.push_back(entity);
        _ghostDirections.push_back(Input::UP);
        _ghostSpawnPositions.push_back(entity.position);
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
