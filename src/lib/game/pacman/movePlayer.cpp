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
struct MapSize {
  int height;
  int width;
};

Position getNextPosition(const Position& pos, Input direction,
                         MapSize mapSize) {
  float newX = pos.x;
  float newY = pos.y;

  switch (direction) {
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
      return pos;
  }
  if (newY < 0) {
    newY = static_cast<float>(mapSize.height - 1);
  } else if (newY >= static_cast<float>(mapSize.height)) {
    newY = 0;
  }
  if (newX < 0) {
    newX = static_cast<float>(mapSize.width - 1);
  } else if (newX >= static_cast<float>(mapSize.width)) {
    newX = 0;
  }
  return Position{.x = newX, .y = newY};
}
}  // namespace

void Pacman::movePlayer(Input input) {
  if (input == Input::UP || input == Input::DOWN || input == Input::LEFT ||
      input == Input::RIGHT) {
    _bufferedDirection = input;
  }

  if (_playerMovementTimer < PLAYER_MOVE_DELAY) {
    return;
  }
  _playerMovementTimer -= PLAYER_MOVE_DELAY;

  const MapSize mapSize = {
      .height = static_cast<int>(_map.size()),
      .width =
          static_cast<int>(_map[static_cast<int>(_player.position.y)].size())};

  auto canMoveTo = [this](const Position& pos) {
    const type tile = getTile(static_cast<int>(pos.y), static_cast<int>(pos.x));
    return tile != type::WALL && tile != type::GHOST_DOOR;
  };

  if (_bufferedDirection != Input::NONE) {
    const Position bufferedPos =
        getNextPosition(_player.position, _bufferedDirection, mapSize);
    if (canMoveTo(bufferedPos)) {
      _lastPlayerInput = _bufferedDirection;
      _bufferedDirection = Input::NONE;
      _player.position = bufferedPos;
      return;
    }
  }

  if (_lastPlayerInput != Input::NONE) {
    const Position currentPos =
        getNextPosition(_player.position, _lastPlayerInput, mapSize);
    if (canMoveTo(currentPos)) {
      _player.position = currentPos;
    }
  }
}
