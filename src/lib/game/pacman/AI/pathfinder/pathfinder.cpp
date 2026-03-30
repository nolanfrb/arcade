/*
** EPITECH PROJECT, 2026
** arcade
** File description:
** pathfinder
*/

#include "pathfinder.hpp"
#include <cmath>
#include <vector>
#include "../../../../../shared/Position.hpp"
#include "../../pacman.hpp"
#include "node.hpp"

namespace {
const Node* findNode(const std::vector<Node>& nodes, const Node& target) {
  for (const Node& node : nodes) {
    if (node == target) {
      return &node;
    }
  }
  return nullptr;
}

std::vector<Node>::iterator findNode(std::vector<Node>& nodes,
                                     const Node& target) {
  for (auto it = nodes.begin(); it != nodes.end(); ++it) {
    if (*it == target) {
      return it;
    }
  }
  return nodes.end();
}

void reversePath(std::vector<Position>& path) {
  for (std::size_t left = 0, right = path.size(); left < right / 2; ++left) {
    Position tmp = path[left];
    path[left] = path[right - 1 - left];
    path[right - 1 - left] = tmp;
  }
}

std::vector<Position> createPath(Node current,
                                 const std::vector<Node>& closedSet) {
  std::vector<Position> path;
  path.push_back(Position{.x = static_cast<float>(current.getX()),
                          .y = static_cast<float>(current.getY())});
  while (current.getParentX() != -1 && current.getParentY() != -1) {
    const Node target(current.getParentX(), current.getParentY());
    const Node* parent = findNode(closedSet, target);
    if (parent != nullptr) {
      path.push_back(Position{.x = static_cast<float>(parent->getX()),
                              .y = static_cast<float>(parent->getY())});
      current = *parent;
    } else {
      break;
    }
  }
  reversePath(path);
  return path;
}

bool checkNeighbor(const Node& neighbor, const std::vector<Node>& closedSet,
                   const std::vector<std::vector<type>>& map,
                   bool canPassDoor) {
  if (findNode(closedSet, neighbor) != nullptr) {
    return false;
  }
  if (neighbor.getY() < 0 || neighbor.getY() >= static_cast<int>(map.size())) {
    return false;
  }
  if (neighbor.getX() < 0 ||
      neighbor.getX() >= static_cast<int>(map[neighbor.getY()].size())) {
    return false;
  }
  const type nodeType = map[neighbor.getY()][neighbor.getX()];
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
  const int tentativeGCost = current.getGCost() + 1;
  auto existingIt = findNode(openSet, neighbor);
  if (existingIt != openSet.end()) {
    if (tentativeGCost < existingIt->getGCost()) {
      existingIt->setParentX(current.getX());
      existingIt->setParentY(current.getY());
      existingIt->setGCost(tentativeGCost);
      existingIt->setHCost(
          std::abs(neighbor.getX() - static_cast<int>(target.x)) +
          std::abs(neighbor.getY() - static_cast<int>(target.y)));
    }
  } else {
    neighbor.setParentX(current.getX());
    neighbor.setParentY(current.getY());
    neighbor.setGCost(tentativeGCost);
    neighbor.setHCost(std::abs(neighbor.getX() - static_cast<int>(target.x)) +
                      std::abs(neighbor.getY() - static_cast<int>(target.y)));
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
  startNode.setHCost(std::abs(static_cast<int>(start.x - target.x)) +
                     std::abs(static_cast<int>(start.y - target.y)));
  openSet.push_back(startNode);
  while (!openSet.empty()) {
    const Node current = findLowerFCostNode(openSet);
    const auto currentIt = findNode(openSet, current);
    if (currentIt != openSet.end()) {
      openSet.erase(currentIt);
    }

    if (current.getX() == static_cast<int>(target.x) &&
        current.getY() == static_cast<int>(target.y)) {
      return createPath(current, closedSet);
    }
    closedSet.push_back(current);
    const std::vector<Node> neighbors = {
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
