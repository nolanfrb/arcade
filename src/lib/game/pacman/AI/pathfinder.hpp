/*
** EPITECH PROJECT, 2026
** arcade
** File description:
** pathfinder
*/

#pragma once

#include <vector>
#include "../../../../shared/Position.hpp"
#include "../pacman.hpp"

class Pathfinder {
 public:
  static std::vector<Position> aStar(Position start,
                                     const std::vector<std::vector<type>>& map,
                                     Position target, bool canPassDoor = false);
};
