/*
** EPITECH PROJECT, 2026
** arcade
** File description:
** pathfinder
*/

#include "pathfinder.hpp"
#include <algorithm>
#include <vector>
#include "../../../../../shared/Position.hpp"
#include "../../pacman.hpp"
#include "node.hpp"

namespace {
std::vector<Position> createPath(Node current,
                                 const std::vector<Node>& closedSet) {
  std::vector<Position> path;
  path.emplace_back(current.getX(), current.getY());
  while (current.getParentX() != -1 && current.getParentY() != -1) {
    Node target(current.getParentX(), current.getParentY());
    auto parent = std::ranges::find(closedSet, target);
    if (parent != closedSet.end()) {
      path.emplace_back(parent->getX(), parent->getY());
      current = *parent;
    } else {
      break;
    }
  }
  std::ranges::reverse(path);
  return path;
}

bool checkNeighbor(const Node& neighbor, const std::vector<Node>& closedSet,
                   const std::vector<std::vector<type>>& map,
                   bool canPassDoor) {
  if (closedSet.end() != std::ranges::find(closedSet, neighbor)) {
    return false;
  }
  if (neighbor.getY() < 0 || neighbor.getY() >= static_cast<int>(map.size())) {
    return false;
  }
  if (neighbor.getX() < 0 ||
      neighbor.getX() >= static_cast<int>(map[neighbor.getY()].size())) {
    return false;
  }
  type nodeType = map[neighbor.getY()][neighbor.getX()];
  if (nodeType == type::WALL) {
    return false;
  }
  if (nodeType == type::GHOST_DOOR && !canPassDoor) {
    return false;
  }
  return true;
}
Node findLowerFCostNode(const std::vector<Node>& openSet) {
  auto lowestFCost = openSet.begin();
  for (auto it = openSet.begin(); it != openSet.end(); ++it) {
    if (it->fCost() < lowestFCost->fCost()) {
      lowestFCost = it;
    }
  }
  return *lowestFCost;
}
void setNeighbor(Node& neighbor, const Node& current,
                 const std::vector<Node>& closedSet,
                 const std::vector<std::vector<type>>& map, bool canPassDoor,
                 std::vector<Node>& openSet, Position target) {
  if (!checkNeighbor(neighbor, closedSet, map, canPassDoor)) {
    return;
  }
  int tentativeGCost = current.getGCost() + 1;
  auto existingIt = std::ranges::find(openSet, neighbor);
  if (existingIt != openSet.end()) {
    if (tentativeGCost < existingIt->getGCost()) {
      existingIt->setParentX(current.getX());
      existingIt->setParentY(current.getY());
      existingIt->setGCost(tentativeGCost);
      existingIt->setHCost(abs(neighbor.getX() - static_cast<int>(target.x)) +
                           abs(neighbor.getY() - static_cast<int>(target.y)));
    }
  } else {
    neighbor.setParentX(current.getX());
    neighbor.setParentY(current.getY());
    neighbor.setGCost(tentativeGCost);
    neighbor.setHCost(abs(neighbor.getX() - static_cast<int>(target.x)) +
                      abs(neighbor.getY() - static_cast<int>(target.y)));
    openSet.push_back(neighbor);
  }
}

}  // namespace

std::vector<Position> Pathfinder::aStar(
    Position start, const std::vector<std::vector<type>>& map, Position target,
    bool canPassDoor) {
  std::vector<Node> openSet;
  std::vector<Node> closedSet;
  Node startNode(static_cast<int>(start.x), static_cast<int>(start.y));
  startNode.setGCost(0);
  startNode.setHCost(abs(static_cast<int>(start.x - target.x)) +
                     abs(static_cast<int>(start.y - target.y)));
  openSet.push_back(startNode);
  while (!openSet.empty()) {
    Node current = findLowerFCostNode(openSet);
    openSet.erase(std::ranges::find(openSet, current));

    if (current.getX() == static_cast<int>(target.x) &&
        current.getY() == static_cast<int>(target.y)) {
      return createPath(current, closedSet);
    }
    closedSet.push_back(current);
    std::vector<Node> neighbors = {
        Node(current.getX() + 1, current.getY()),
        Node(current.getX() - 1, current.getY()),
        Node(current.getX(), current.getY() + 1),
        Node(current.getX(), current.getY() - 1),
    };
    for (Node neighbor : neighbors) {
      setNeighbor(neighbor, current, closedSet, map, canPassDoor, openSet,
                  target);
    }
  }
  return {};
}
