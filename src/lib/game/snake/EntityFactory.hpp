/*
** EPITECH PROJECT, 2026
** arcade
** File description:
** EntityFactory - static utility methods for creating entities and texts
*/

#pragma once

#include <array>
#include <cstdint>
#include <string>
#include "snake.hpp"

class EntityFactory {
 public:
  EntityFactory() = delete;

  static Entity createTile(const Position& pos, const std::string& spritePath,
                           const std::array<std::uint8_t, 4>& color,
                           char asciiChar);

  static Entity createSnakeEntity(const SnakeSegment& segment);

  static Entity createFoodEntity(const FoodItem& food);

  static Text createText(const std::string& content, const Position& pos,
                         const std::array<std::uint8_t, 4>& color);

 private:
  struct SnakeSegmentInfo {
    std::string spritePath;  // NOLINT(readability-redundant-member-init)
    std::array<std::uint8_t, 4> color{};
    char asciiChar = '\0';
  };

  static SnakeSegmentInfo getSegmentInfo(EntityTypeSnake type);
};
