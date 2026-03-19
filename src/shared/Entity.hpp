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

struct Entity {
  Position position;
  char asciiChar;
  std::string texturePath;
  std::array<uint8_t, 4> color;
  bool isPlayer;
};
