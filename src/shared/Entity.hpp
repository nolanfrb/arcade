/*
** EPITECH PROJECT, 2026
** Arcade
** File description:
** Entity
*/

#pragma once

#include <array>
#include <cstdint>
#include <string>
#include "Position.hpp"

enum class Shape : std::int8_t { RECTANGLE, CIRCLE, TRIANGLE, SPRITE };

class Entity {
 public:
  Shape type;
  Position position;
  float width;
  float height;
  char asciiChar;
  std::array<uint8_t, 4> color;
  std::string spritePath;
  bool isTextInput;
};
