/*
** EPITECH PROJECT, 2026
** Arcade
** File description:
** Sound
*/

#pragma once

#include <string>

class Sound {
 public:
  std::string filePath;
  float volume = 100.F;
  bool loop = false;
};
