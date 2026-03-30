/*
** EPITECH PROJECT, 2026
** arcade
** File description:
** utils
*/

#pragma once

#include <array>
#include <cstdint>
#include <vector>
#include "../../../../shared/Entity.hpp"
#include "../../../../shared/Position.hpp"
#include "../pacman.hpp"

class Utils {
 public:
  static int randomNumber(int min, int max);
  static bool hasColor(const Entity& ghost,
                       const std::array<uint8_t, 4>& color);
  static Position clampToMap(const std::vector<std::vector<type>>& map,
                             Position target);
  static bool isWalkableTile(type tile, bool canPassDoor);
};
