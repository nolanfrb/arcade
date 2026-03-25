/*
** EPITECH PROJECT, 2026
** arcade
** File description:
** pacman
*/

#include "pacman.hpp"
#include <fstream>
#include <string>

bool Pacman::loadMap(const std::string& filePath) { return true; }

void Pacman::init() {
  setName("Pacman");
  if (!loadMap("assets/pacman/pacman_map.txt")) {
    // Handle error
  }
}
