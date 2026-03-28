/*
** EPITECH PROJECT, 2026
** arcade
** File description:
** movement
*/

#include "ghostMovement.hpp"
#include "../../../../../shared/Entity.hpp"
#include "../../../../../shared/Input.hpp"
#include "../../../../../shared/Position.hpp"
#include "../../pacman.hpp"
#include "../../utils/utils.hpp"

Position ghostMovement::findFarthestPoint(
    const std::vector<std::vector<type>>& map, Position player) {
  Position bestTarget(player.x, player.y);
  int maxDistance = -1;

  for (int rowIndex = 0; rowIndex < static_cast<int>(map.size()); ++rowIndex) {
    for (int colIndex = 0; colIndex < static_cast<int>(map[rowIndex].size());
         ++colIndex) {
      if (!Utils::isWalkableTile(map[rowIndex][colIndex], true)) {
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

Position ghostMovement::moveBlueGhostPivotTarget(Position target,
                                                 Input playerInput) {
  if (playerInput == Input::UP) {
    target.y -= 2;
  } else if (playerInput == Input::DOWN) {
    target.y += 2;
  } else if (playerInput == Input::LEFT) {
    target.x -= 2;
  } else if (playerInput == Input::RIGHT) {
    target.x += 2;
  }
  return Position(target.x, target.y);
}

Position ghostMovement::movePinkGhostTarget(Position target,
                                            Input playerInput) {
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

Position ghostMovement::moveBlueGhostTarget(Position target,
                                            const Entity& redGhost,
                                            Input playerInput) {
  Position pivotPoint = moveBlueGhostPivotTarget(target, playerInput);
  target.x = pivotPoint.x + (pivotPoint.x - redGhost.position.x);
  target.y = pivotPoint.y + (pivotPoint.y - redGhost.position.y);
  return Position(target.x, target.y);
}

Position ghostMovement::moveOrangeGhostTarget(
    const Entity& ghost, Position target,
    const std::vector<std::vector<type>>& map) {
  int distanceToPlayer =
      std::abs(static_cast<int>(ghost.position.x - target.x)) +
      std::abs(static_cast<int>(ghost.position.y - target.y));
  if (distanceToPlayer < ORANGE_SAFE_DISTANCE) {
    target = Utils::clampToMap(map, target);
  }
  return Position(target.x, target.y);
}
