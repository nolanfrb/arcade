/*
** EPITECH PROJECT, 2026
** arcade
** File description:
** movePlayer
*/

#include "../../../shared/Entity.hpp"
#include "../../../shared/Input.hpp"
#include "../../../shared/Position.hpp"
#include "pacman.hpp"

namespace {
const std::string& getPacmanSpritePath(Input input) {
  static const std::string defaultPath = "assets/pacman/pacman_move_right.png";
  static const std::string upPath = "assets/pacman/pacman_move_top.png";
  static const std::string downPath = "assets/pacman/pacman_move_bottom.png";
  static const std::string leftPath = "assets/pacman/pacman_move_left.png";
  static const std::string rightPath = "assets/pacman/pacman_move_right.png";

  switch (input) {
    case Input::UP:
      return upPath;
    case Input::DOWN:
      return downPath;
    case Input::LEFT:
      return leftPath;
    case Input::RIGHT:
      return rightPath;
    default:
      return defaultPath;
  }
}
}  // namespace

void Pacman::movePlayer(Input input) {
  if (input != Input::UP && input != Input::DOWN && input != Input::LEFT &&
      input != Input::RIGHT) {
    return;
  }
  _player.type.spritePath = getPacmanSpritePath(input);
  if (_playerMovementTimer < PLAYER_MOVE_DELAY) {
    return;
  }
  _playerMovementTimer -= PLAYER_MOVE_DELAY;

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
