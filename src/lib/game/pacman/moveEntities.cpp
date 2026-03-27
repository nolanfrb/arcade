/*
** EPITECH PROJECT, 2026
** arcade
** File description:
** moveEntities
*/

#include <algorithm>
#include <vector>
#include "../../../shared/Entity.hpp"
#include "../../../shared/Input.hpp"
#include "../../../shared/Position.hpp"
#include "AI/pathfinder.hpp"
#include "pacman.hpp"

void Pacman::movePlayer(Input input) {
  float newX = _player.position.x;
  float newY = _player.position.y;
  const int mapHeight = static_cast<int>(_map.size());

  switch (input) {
    case Input::UP:
      newY -= 1;
      break;
    case Input::DOWN:
      newY += 1;
      break;
    case Input::LEFT:
      newX -= 1;
      break;
    case Input::RIGHT:
      newX += 1;
      break;
    default:
      return;
  }
  if (newY < 0) {
    newY = static_cast<float>(mapHeight - 1);
  } else if (newY >= static_cast<float>(mapHeight)) {
    newY = 0;
  }
  const int mapWidth = static_cast<int>(_map[static_cast<int>(newY)].size());
  if (newX < 0) {
    newX = static_cast<float>(mapWidth - 1);
  } else if (newX >= static_cast<float>(mapWidth)) {
    newX = 0;
  }
  if (getTile(static_cast<int>(newY), static_cast<int>(newX)) != type::WALL &&
      getTile(static_cast<int>(newY), static_cast<int>(newX)) !=
          type::GHOST_DOOR) {
    _player.position.x = newX;
    _player.position.y = newY;
  }
}

void Pacman::moveDeadGhosts(float deltaTime) {}

void Pacman::moveGhosts(float deltaTime) {
  _ghostMovementTimer += deltaTime;
  if (_ghostMovementTimer < 0.3) {
    return;
  }
  _ghostMovementTimer = 0;

  bool canPassDoor = _gameTimer >= 3;
  for (auto& ghost : _ghosts) {
    if (std::ranges::find_if(_deadGhosts.begin(), _deadGhosts.end(),
                             [&ghost](const Entity& deadGhost) {
                               return deadGhost.position.x ==
                                          ghost.position.x &&
                                      deadGhost.position.y == ghost.position.y;
                             }) != _deadGhosts.end()) {
      continue;
    }
    std::vector<Position> path = Pathfinder::aStar(
        Position((ghost.position.x), (ghost.position.y)), _map,
        Position((_player.position.x), (_player.position.y)), canPassDoor);
    if (path.size() > 1) {
      Position nextPos = path[1];
      ghost.position.x = nextPos.x;
      ghost.position.y = nextPos.y;
    } else if (path.size() == 1) {
      Position nextPos = path[0];
      ghost.position.x = nextPos.x;
      ghost.position.y = nextPos.y;
    }
  }
  moveDeadGhosts(deltaTime);
}
