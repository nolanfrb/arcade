/*
** EPITECH PROJECT, 2026
** Arcade
** File description:
** Position
*/

#pragma once

#include <optional>

struct Position {
  float x = 0;
  float y = 0;
  std::optional<float> z = std::nullopt;
};
