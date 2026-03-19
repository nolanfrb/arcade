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

struct Text {
  Position position;
  std::string text;
  std::array<uint8_t, 4> color;
};
