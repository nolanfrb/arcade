/*
** EPITECH PROJECT, 2026
** arcade
** File description:
** map
*/
#include <cstddef>
#include <fstream>
#include <string>
#include <vector>
#include "pacman.hpp"

namespace {
type translateTile(char character) {
  switch (character) {
    case '#':
      return type::WALL;
    case 'C':
      return type::PACMAN;
    case 'R':
      return type::RED_GHOST;
    case 'P':
      return type::PINK_GHOST;
    case 'B':
      return type::BLUE_GHOST;
    case 'O':
      return type::ORANGE_GHOST;
    case 'S':
      return type::SUPERPACGUM;
    case '.':
      return type::PACGUM;
    case 'F':
      return type::FOOD;
    case '|':
      return type::GHOST_DOOR;
    default:
      return type::EMPTY;
  }
}
}  // namespace

bool Pacman::loadMap(const std::string& filePath) {
  std::ifstream file(filePath);
  if (!file.is_open()) {
    return false;
  }
  std::string line;
  while (std::getline(file, line)) {
    std::vector<type> row;
    for (const char character : line) {
      row.emplace_back(translateTile(character));
    }
    _map.emplace_back(row);
  }
  return true;
}

type Pacman::getTile(int xCoordinate, int yCoordinate) {
  if (xCoordinate < 0 || xCoordinate >= static_cast<int>(_map.size()) ||
      yCoordinate < 0 ||
      yCoordinate >= static_cast<int>(_map[xCoordinate].size())) {
    return type::WALL;
  }
  return _map[xCoordinate][yCoordinate];
}

bool Pacman::checkMap() {
  std::size_t pacmanCount = 0;
  std::size_t ghostCount = 0;
  for (int xCoordinate = 0; xCoordinate < _map.size(); xCoordinate++) {
    for (int yCoordinate = 0; yCoordinate < _map[xCoordinate].size();
         yCoordinate++) {
      if (getTile(xCoordinate, yCoordinate) == type::PACMAN) {
        pacmanCount++;
      } else if (getTile(xCoordinate, yCoordinate) == type::RED_GHOST ||
                 getTile(xCoordinate, yCoordinate) == type::PINK_GHOST ||
                 getTile(xCoordinate, yCoordinate) == type::BLUE_GHOST ||
                 getTile(xCoordinate, yCoordinate) == type::ORANGE_GHOST) {
        ghostCount++;
      }
    }
  }
  return pacmanCount == 1 && ghostCount >= 1 && ghostCount <= 4;
}
