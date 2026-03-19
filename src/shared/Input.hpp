/*
** EPITECH PROJECT, 2026
** Arcade
** File description:
** Input
*/

#pragma once

#include <cstdint>

enum class Input : std::int8_t {
  NONE,
  UP,
  DOWN,
  LEFT,
  RIGHT,
  ACTION,
  MENU,
  EXIT,
  RESTART
};
