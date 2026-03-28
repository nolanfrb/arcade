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
#include "AI/ghostMovement/ghostMovement.hpp"
#include "AI/pathfinder/pathfinder.hpp"
#include "pacman.hpp"
#include "utils/utils.hpp"

void Pacman::moveDeadGhosts(Entity& ghost) {
  if (_ghostSpawnPositions.empty()) {
    return;
  }
  std::vector<Position> path = Pathfinder::aStar(
      Position((ghost.position.x), (ghost.position.y)), _map,
      Position((_ghostSpawnPositions[0].x +
                static_cast<float>(Utils::randomNumber(0, 3))),
               (_ghostSpawnPositions[0].y +
                static_cast<float>(Utils::randomNumber(-1, 2)))),
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
    bool isBlueGhost = false;
    if (Utils::hasColor(ghost, PINK)) {
      ghostTarget = ghostMovement::movePinkGhostTarget(target, playerInput);
    } else if (Utils::hasColor(ghost, BLUE)) {
      isBlueGhost = true;
      Entity redGhost = ghost;
      for (const auto& currentGhost : _ghosts) {
        if (Utils::hasColor(currentGhost, RED)) {
          redGhost = currentGhost;
          break;
        }
      }
      ghostTarget =
          ghostMovement::moveBlueGhostTarget(target, redGhost, playerInput);
    } else if (Utils::hasColor(ghost, ORANGE)) {
      ghostTarget = ghostMovement::moveOrangeGhostTarget(ghost, target, _map);
    }
    ghostTarget = Utils::clampToMap(_map, ghostTarget);
    std::vector<Position> path =
        Pathfinder::aStar(Position((ghost.position.x), (ghost.position.y)),
                          _map, ghostTarget, canPassDoor);
    if (path.empty() && isBlueGhost) {
      Position pivotTarget =
          ghostMovement::moveBlueGhostPivotTarget(target, playerInput);
      pivotTarget = Utils::clampToMap(_map, pivotTarget);
      path = Pathfinder::aStar(Position((ghost.position.x), (ghost.position.y)),
                               _map, pivotTarget, canPassDoor);
    }
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
    target = ghostMovement::findFarthestPoint(
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
