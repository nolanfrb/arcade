/*
** EPITECH PROJECT, 2026
** arcade
** File description:
** utils
*/

#include "utils.hpp"
#include <random>
#include <vector>
#include "../../../../shared/Entity.hpp"
#include "../../../../shared/Position.hpp"
#include "../pacman.hpp"

int Utils::randomNumber(int min, int max) {
  static std::random_device randomDevice;
  static std::mt19937 generator(randomDevice());
  std::uniform_int_distribution<int> distribution(min, max);
  return distribution(generator);
}

bool Utils::hasColor(const Entity& ghost, const std::array<uint8_t, 4>& color) {
  return ghost.type.color == color;
}

bool Utils::isWalkableTile(type tile, bool canPassDoor) {
  if (tile == type::WALL) {
    return false;
  }
  if (tile == type::GHOST_DOOR && !canPassDoor) {
    return false;
  }
  return true;
}

Position Utils::clampToMap(const std::vector<std::vector<type>>& map,
                           Position target) {
  if (map.empty()) {
    return Position{.x = 0, .y = 0};
  }
  if (target.y < 0) {
    target.y = 0;
  } else if (target.y >= static_cast<float>(map.size())) {
    target.y = static_cast<float>(map.size() - 1);
  }
  const int rowIndex = static_cast<int>(target.y);
  if (target.x < 0) {
    target.x = 0;
  } else if (target.x >= static_cast<float>(map[rowIndex].size())) {
    target.x = static_cast<float>(map[rowIndex].size() - 1);
  }
  return target;
}
