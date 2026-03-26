/*
** EPITECH PROJECT, 2026
** arcade
** File description:
** pathfinder
*/

#pragma once

#include <vector>
#include "../../../../shared/Position.hpp"
#include "../pacman.hpp"

class Node {
  int x;
  int y;
  int gCost;
  int hCost;
  int parentX;
  int parentY;

 public:
  [[nodiscard]] int getX() const { return x; }
  [[nodiscard]] int getY() const { return y; }
  [[nodiscard]] int getGCost() const { return gCost; }
  [[nodiscard]] int getHCost() const { return hCost; }
  [[nodiscard]] int getParentX() const { return parentX; }
  [[nodiscard]] int getParentY() const { return parentY; }

  void setGCost(int gCost) { this->gCost = gCost; }
  void setHCost(int hCost) { this->hCost = hCost; }
  void setParentX(int parentX) { this->parentX = parentX; }
  void setParentY(int parentY) { this->parentY = parentY; }

  [[nodiscard]] int fCost() const { return gCost + hCost; }
  bool operator>(const Node& other) const { return fCost() > other.fCost(); }
};

class Pathfinder {
 public:
  static std::vector<Position> aStar(Position start,
                                     const std::vector<std::vector<type>>& map,
                                     Position target, bool canPassDoor = false);
};
