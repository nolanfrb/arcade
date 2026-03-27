/*
** EPITECH PROJECT, 2026
** arcade
** File description:
** moveEntities
*/

#include <vector>
#include "../../../shared/Entity.hpp"
#include "../../../shared/Input.hpp"
#include "../../../shared/Position.hpp"
#include "AI/pathfinder.hpp"
#include "pacman.hpp"

namespace {
bool isWalkableTile(type tile, bool canPassDoor) {
  if (tile == type::WALL) {
    return false;
  }
  if (tile == type::GHOST_DOOR && !canPassDoor) {
    return false;
  }
  return true;
}
Position findFarthestPoint(const std::vector<std::vector<type>>& map,
                           Position player) {
  Position bestTarget(player.x, player.y);
  int maxDistance = -1;

  for (int rowIndex = 0; rowIndex < static_cast<int>(map.size()); ++rowIndex) {
    for (int colIndex = 0; colIndex < static_cast<int>(map[rowIndex].size());
         ++colIndex) {
      if (!isWalkableTile(map[rowIndex][colIndex], true)) {
        continue;
      }

      const int distance = std::abs(colIndex - static_cast<int>(player.x)) +
                           std::abs(rowIndex - static_cast<int>(player.y));
      if (distance > maxDistance) {
        maxDistance = distance;
        bestTarget = Position(static_cast<float>(colIndex),
                              static_cast<float>(rowIndex));
      }
    }
  }
  return bestTarget;
}

}  // namespace

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

void Pacman::moveDeadGhosts(Entity& ghost) {
  if (_deadGhostMovementTimer < GHOST_SPEED_DEAD) {
    return;
  }
  _deadGhostMovementTimer = 0;
  _ghostSpeed = GHOST_SPEED_DEAD;
  if (_ghostSpawnPositions.empty()) {
    return;
  }
  std::vector<Position> path = Pathfinder::aStar(
      Position((ghost.position.x), (ghost.position.y)), _map,
      Position((_ghostSpawnPositions[0].x), (_ghostSpawnPositions[0].y)), true);
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

void Pacman::moveAliveGhosts(Position target, bool canPassDoor) {
  if (_aliveGhostMovementTimer < _ghostSpeed) {
    return;
  }
  _aliveGhostMovementTimer = 0;
  _ghostSpeed =
      _isSuperPacgumActive ? GHOST_SPEED_FRIGHTENED : GHOST_SPEED_NORMAL;
  for (auto& ghost : _ghosts) {
    std::vector<Position> path =
        Pathfinder::aStar(Position((ghost.position.x), (ghost.position.y)),
                          _map, Position((target.x), (target.y)), canPassDoor);
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
}

void Pacman::moveGhosts(float deltaTime) {
  _aliveGhostMovementTimer += deltaTime;
  _deadGhostMovementTimer += deltaTime;

  bool canPassDoor = _gameTimer >= 3;
  Position target(_player.position.x, _player.position.y);
  if (_isSuperPacgumActive) {
    canPassDoor = false;
    target = findFarthestPoint(
        _map, Position(_player.position.x, _player.position.y));
  }
  moveAliveGhosts(target, canPassDoor);
  for (auto& deadGhost : _deadGhosts) {
    moveDeadGhosts(deadGhost);
  }
}
