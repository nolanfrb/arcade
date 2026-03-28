/*
** EPITECH PROJECT, 2026
** arcade
** File description:
** moveEntities
*/

#include <array>
#include <random>
#include <vector>
#include "../../../shared/Entity.hpp"
#include "../../../shared/Input.hpp"
#include "../../../shared/Position.hpp"
#include "AI/pathfinder.hpp"
#include "pacman.hpp"

constexpr int ORANGE_SAFE_DISTANCE = 8;

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

int randomNumber(int min, int max) {
  static std::random_device randomDevice;
  static std::mt19937 generator(randomDevice());
  std::uniform_int_distribution<int> distribution(min, max);
  return distribution(generator);
}

bool hasColor(const Entity& ghost, const std::array<uint8_t, 4>& color) {
  return ghost.type.color == color;
}

Position movePinkGhostTarget(Position target, Input playerInput) {
  if (playerInput == Input::UP) {
    target.y -= 4;
  } else if (playerInput == Input::DOWN) {
    target.y += 4;
  } else if (playerInput == Input::LEFT) {
    target.x -= 4;
  } else if (playerInput == Input::RIGHT) {
    target.x += 4;
  }
  return Position(target.x, target.y);
}

Position moveBlueGhostTarget(Position target, const Entity& redGhost,
                             Input playerInput) {
  Position pivotPoint = target;
  if (playerInput == Input::UP) {
    pivotPoint.y -= 2;
  } else if (playerInput == Input::DOWN) {
    pivotPoint.y += 2;
  } else if (playerInput == Input::LEFT) {
    pivotPoint.x -= 2;
  } else if (playerInput == Input::RIGHT) {
    pivotPoint.x += 2;
  }
  target.x = pivotPoint.x + (pivotPoint.x - redGhost.position.x);
  target.y = pivotPoint.y + (pivotPoint.y - redGhost.position.y);
  return Position(target.x, target.y);
}

Position moveOrangeGhostTarget(const Entity& ghost, Position target) {
  int distanceToPlayer =
      std::abs(static_cast<int>(ghost.position.x - target.x)) +
      std::abs(static_cast<int>(ghost.position.y - target.y));
  if (distanceToPlayer < ORANGE_SAFE_DISTANCE) {
    target.x = 0;
    target.y = 0;
  }
  return Position(target.x, target.y);
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
  if (_ghostSpawnPositions.empty()) {
    return;
  }
  std::vector<Position> path = Pathfinder::aStar(
      Position((ghost.position.x), (ghost.position.y)), _map,
      Position(
          (_ghostSpawnPositions[0].x + static_cast<float>(randomNumber(0, 3))),
          (_ghostSpawnPositions[0].y +
           static_cast<float>(randomNumber(-1, 2)))),
      true);
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

void Pacman::moveAliveGhosts(Position target, bool canPassDoor,
                             Input playerInput) {
  _ghostSpeed =
      _isSuperPacgumActive ? GHOST_SPEED_FRIGHTENED : GHOST_SPEED_NORMAL;
  if (_aliveGhostMovementTimer < _ghostSpeed) {
    return;
  }
  _aliveGhostMovementTimer = 0;
  for (auto& ghost : _ghosts) {
    Position ghostTarget = target;
    if (hasColor(ghost, PINK)) {
      ghostTarget = movePinkGhostTarget(target, playerInput);
    } else if (hasColor(ghost, BLUE)) {
      Entity redGhost = ghost;
      for (const auto& currentGhost : _ghosts) {
        if (hasColor(currentGhost, RED)) {
          redGhost = currentGhost;
          break;
        }
      }
      ghostTarget = moveBlueGhostTarget(target, redGhost, playerInput);
    } else if (hasColor(ghost, ORANGE)) {
      ghostTarget = moveOrangeGhostTarget(ghost, target);
    }

    std::vector<Position> path =
        Pathfinder::aStar(Position((ghost.position.x), (ghost.position.y)),
                          _map, ghostTarget, canPassDoor);

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

void Pacman::moveGhosts(float deltaTime, Input playerInput) {
  _aliveGhostMovementTimer += deltaTime;
  _deadGhostMovementTimer += deltaTime;

  bool canPassDoor = _gameTimer >= 3;
  Position target(_player.position.x, _player.position.y);
  if (_isSuperPacgumActive) {
    canPassDoor = false;
    target = findFarthestPoint(
        _map, Position(_player.position.x, _player.position.y));
  }
  moveAliveGhosts(target, canPassDoor, playerInput);
  if (_deadGhostMovementTimer >= GHOST_SPEED_DEAD) {
    _deadGhostMovementTimer = 0;
    for (auto& deadGhost : _deadGhosts) {
      moveDeadGhosts(deadGhost);
    }
  }
}
