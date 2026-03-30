/*
** EPITECH PROJECT, 2026
** arcade
** File description:
** moveEntities
*/

#include <cstddef>
#include <vector>
#include "../../../shared/Entity.hpp"
#include "../../../shared/Input.hpp"
#include "../../../shared/Position.hpp"
#include "AI/ghostMovement/ghostMovement.hpp"
#include "AI/pathfinder/pathfinder.hpp"
#include "pacman.hpp"
#include "utils/utils.hpp"

namespace {
void respawnGhosts(float deltaTime, std::vector<Entity>& deadGhosts,
                   std::vector<float>& ghostRespawnTimers,
                   std::vector<Entity>& ghosts,
                   std::vector<Input>& ghostDirections,
                   const std::vector<Position>& ghostSpawnPositions) {
  for (std::size_t i = 0;
       i < deadGhosts.size() && i < ghostRespawnTimers.size();) {
    ghostRespawnTimers[i] += deltaTime;
    if (ghostRespawnTimers[i] >= GHOST_RESPAWN_DELAY) {
      if (!ghostSpawnPositions.empty()) {
        deadGhosts[i].position = ghostSpawnPositions[0];
      }
      ghosts.push_back(deadGhosts[i]);
      ghostDirections.push_back(Input::UP);
      deadGhosts.erase(deadGhosts.begin() +
                       static_cast<std::vector<Entity>::difference_type>(i));
      ghostRespawnTimers.erase(
          ghostRespawnTimers.begin() +
          static_cast<std::vector<float>::difference_type>(i));
      continue;
    }
    ++i;
  }
}
}  // namespace

void Pacman::moveDeadGhosts(Entity& ghost) {
  if (_ghostSpawnPositions.empty()) {
    return;
  }
  std::vector<Position> path = Pathfinder::aStar(
      Position{.x = (ghost.position.x), .y = (ghost.position.y)}, _map,
      Position{.x = (_ghostSpawnPositions[0].x +
                     static_cast<float>(Utils::randomNumber(0, 3))),
               .y = (_ghostSpawnPositions[0].y +
                     static_cast<float>(Utils::randomNumber(-1, 2)))},
      true);
  if (path.size() > 1) {
    const Position nextPos = path[1];
    ghost.position.x = nextPos.x;
    ghost.position.y = nextPos.y;
  } else if (path.size() == 1) {
    const Position nextPos = path[0];
    ghost.position.x = nextPos.x;
    ghost.position.y = nextPos.y;
  }
}

void Pacman::moveAliveGhosts(Position target, bool canPassDoor,
                             Input playerInput) {
  float ghostSpeed =
      _isSuperPacgumActive ? GHOST_SPEED_FRIGHTENED : GHOST_SPEED_NORMAL;
  ghostSpeed /= _ghostSpeedMultiplier;
  if (_aliveGhostMovementTimer < ghostSpeed) {
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
    std::vector<Position> path = Pathfinder::aStar(
        Position{.x = (ghost.position.x), .y = (ghost.position.y)}, _map,
        ghostTarget, canPassDoor);
    if (path.empty() && isBlueGhost) {
      Position pivotTarget =
          ghostMovement::moveBlueGhostPivotTarget(target, playerInput);
      pivotTarget = Utils::clampToMap(_map, pivotTarget);
      path = Pathfinder::aStar(
          Position{.x = (ghost.position.x), .y = (ghost.position.y)}, _map,
          pivotTarget, canPassDoor);
    }
    if (path.size() > 1) {
      const Position nextPos = path[1];
      ghost.position.x = nextPos.x;
      ghost.position.y = nextPos.y;
    } else if (path.size() == 1) {
      const Position nextPos = path[0];
      ghost.position.x = nextPos.x;
      ghost.position.y = nextPos.y;
    }
  }
}

void Pacman::moveGhosts(float deltaTime, Input playerInput) {
  _aliveGhostMovementTimer += deltaTime;
  _deadGhostMovementTimer += deltaTime;

  bool canPassDoor = _gameTimer >= GHOST_SPAWN_DELAY;
  Position target{.x = _player.position.x, .y = _player.position.y};
  if (_isSuperPacgumActive) {
    canPassDoor = false;
    target = ghostMovement::findFarthestPoint(
        _map, Position{.x = _player.position.x, .y = _player.position.y});
  }
  moveAliveGhosts(target, canPassDoor, playerInput);
  if (_deadGhostMovementTimer >= GHOST_SPEED_DEAD) {
    _deadGhostMovementTimer = 0;
    for (auto& deadGhost : _deadGhosts) {
      moveDeadGhosts(deadGhost);
    }
  }
  respawnGhosts(deltaTime, _deadGhosts, _ghostRespawnTimers, _ghosts,
                _ghostDirections, _ghostSpawnPositions);
}
