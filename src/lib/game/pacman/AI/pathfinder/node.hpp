/*
** EPITECH PROJECT, 2026
** arcade
** File description:
** node
*/

#pragma once

class Node {
  int x;
  int y;
  int gCost;
  int hCost;
  int parentX;
  int parentY;

 public:
  Node() : x(0), y(0), gCost(0), hCost(0), parentX(-1), parentY(-1) {}
  // NOLINTNEXTLINE
  Node(int x, int y)
      : x(x), y(y), gCost(0), hCost(0), parentX(-1), parentY(-1) {}

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
  bool operator==(const Node& other) const {
    return x == other.x && y == other.y;
  }
};
