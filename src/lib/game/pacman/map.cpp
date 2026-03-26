/*
** EPITECH PROJECT, 2026
** arcade
** File description:
** map
*/

#include <fstream>
#include "pacman.hpp"

namespace {
type translateTile(char character) {
  switch (character) {
    case '#':
      return type::WALL;
    case 'P':
      return type::PACMAN;
    case 'G':
      return type::GHOST;
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
    for (char character : line) {
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
  size_t pacmanCount = 0;
  size_t ghostCount = 0;
  for (int xCoordinate = 0; xCoordinate < _map.size(); xCoordinate++) {
    for (int yCoordinate = 0; yCoordinate < _map[xCoordinate].size();
         yCoordinate++) {
      if (getTile(xCoordinate, yCoordinate) == type::PACMAN) {
        pacmanCount++;
      } else if (getTile(xCoordinate, yCoordinate) == type::GHOST) {
        ghostCount++;
      }
    }
  }
  return pacmanCount == 1 && ghostCount >= 1 && ghostCount <= 4;
}
