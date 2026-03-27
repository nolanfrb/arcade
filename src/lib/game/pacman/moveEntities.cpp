/*
** EPITECH PROJECT, 2026
** arcade
** File description:
** moveEntities
*/

#include <vector>
#include "../../../shared/Entity.hpp"
#include "../../../shared/Input.hpp"
#include "../../../shared/Position.hpp"
#include "pacman.hpp"

void Pacman::movePlayer(Input input) {
  float newX = _player.position.x;
  float newY = _player.position.y;
  const int mapHeight = static_cast<int>(_map.size());

  switch (input) {
    case Input::UP:
      newY -= 1;
      break;
    case Input::DOWN:
      newY += 1;
      break;
    case Input::LEFT:
      newX -= 1;
      break;
    case Input::RIGHT:
      newX += 1;
      break;
    default:
      return;
  }
  if (newY < 0) {
    newY = static_cast<float>(mapHeight - 1);
  } else if (newY >= static_cast<float>(mapHeight)) {
    newY = 0;
  }
  const int mapWidth = static_cast<int>(_map[static_cast<int>(newY)].size());
  if (newX < 0) {
    newX = static_cast<float>(mapWidth - 1);
  } else if (newX >= static_cast<float>(mapWidth)) {
    newX = 0;
  }
  if (getTile(static_cast<int>(newY), static_cast<int>(newX)) != type::WALL &&
      getTile(static_cast<int>(newY), static_cast<int>(newX)) !=
          type::GHOST_DOOR) {
    _player.position.x = newX;
    _player.position.y = newY;
  }
}

void Pacman::moveDeadGhosts(float deltaTime) {}

void Pacman::moveGhosts(float deltaTime) { moveDeadGhosts(deltaTime); }
