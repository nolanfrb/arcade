/*
** EPITECH PROJECT, 2026
** Arcade
** File description:
** Text
*/

#pragma once

#include <array>
#include <cstdint>
#include <string>
#include "Position.hpp"

class Text {
 public:
  std::string content;
  Position position;
  std::array<uint8_t, 4> color;
  std::string fontPath;
  int fontSize;
};
