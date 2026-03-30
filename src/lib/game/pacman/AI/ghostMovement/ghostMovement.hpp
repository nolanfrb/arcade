/*
** EPITECH PROJECT, 2026
** arcade
** File description:
** ghostMovement
*/

#pragma once

#include <vector>
#include "../../../../../shared/Entity.hpp"
#include "../../../../../shared/Input.hpp"
#include "../../../../../shared/Position.hpp"
#include "../../pacman.hpp"

constexpr int ORANGE_SAFE_DISTANCE = 8;

class ghostMovement {
 public:
  static Position findFarthestPoint(const std::vector<std::vector<type>>& map,
                                    Position player);
  static Position moveBlueGhostPivotTarget(Position target, Input playerInput);
  static Position movePinkGhostTarget(Position target, Input playerInput);
  static Position moveBlueGhostTarget(Position target, const Entity& redGhost,
                                      Input playerInput);
  static Position moveOrangeGhostTarget(
      const Entity& ghost, Position target,
      const std::vector<std::vector<type>>& map);
};
