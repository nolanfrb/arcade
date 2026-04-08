/*
** EPITECH PROJECT, 2026
** arcade
** File description:
** snake movement
*/

#include <algorithm>
#include <cmath>
#include <cstdlib>
#include "snake.hpp"

EntityTypeSnake Snake::getHeadTypeForDirection() {
  if (_currentDirection == Input::UP) {
    return EntityTypeSnake::SNAKE_HEAD_UP;
  } else if (_currentDirection == Input::DOWN) {
    return EntityTypeSnake::SNAKE_HEAD_DOWN;
  } else if (_currentDirection == Input::LEFT) {
    return EntityTypeSnake::SNAKE_HEAD_LEFT;
  }
  return EntityTypeSnake::SNAKE_HEAD_RIGHT;
}

Position Snake::getNextHeadPosition() {
  const Position& head = _snake.front().position;
  Position next = head;

  if (_nextDirection == Input::UP) {
    next.y -= 1.F;
  } else if (_nextDirection == Input::DOWN) {
    next.y += 1.F;
  } else if (_nextDirection == Input::LEFT) {
    next.x -= 1.F;
  } else if (_nextDirection == Input::RIGHT) {
    next.x += 1.F;
  }

  return next;
}

bool Snake::isPositionOccupied(const Position& pos, bool includeBody) {
  if (!CYCLICAL &&
      (pos.x <= PLAY_AREA_X - 1.F || pos.x >= PLAY_AREA_X + PLAY_AREA_WIDTH ||
       pos.y <= PLAY_AREA_Y - 1.F || pos.y >= PLAY_AREA_Y + PLAY_AREA_HEIGHT)) {
    return true;
  }

  if (!includeBody) {
    return false;
  }

  for (const auto& segment : _snake) {
    if (std::abs(segment.position.x - pos.x) < 0.1F &&
        std::abs(segment.position.y - pos.y) < 0.1F) {
      return true;
    }
  }

  return false;
}

bool Snake::canMoveThisFrame(float deltaTime) {
  _moveTimer += deltaTime;

  const float speed = _isBoosting ? BOOST_SPEED : _currentSpeed;
  if (_moveTimer < speed) {
    return false;
  }

  _moveTimer = 0.F;
  return true;
}

bool Snake::prepareNextMove(Position& nextHead) {
  _currentDirection = _nextDirection;
  nextHead = getNextHeadPosition();

  if (isPositionOccupied(nextHead, true)) {
    setIsGameOver(true);
    return false;
  }

  _snake.front().type = EntityTypeSnake::SNAKE_BODY;
  _snake.push_front({nextHead, getHeadTypeForDirection()});
  return true;
}

bool Snake::eatFoodAt(const Position& nextHead) {
  for (size_t i = 0; i < _foods.size(); ++i) {
    if (std::abs(_foods[i].position.x - nextHead.x) < 0.1F &&
        std::abs(_foods[i].position.y - nextHead.y) < 0.1F) {
      const int score =
          _foods[i].isBonus ? BONUS_FOOD_SCORE : NORMAL_FOOD_SCORE;
      setScore(getScore() + score);
      _foodEaten++;

      _currentSpeed =
          std::max(MAX_SPEED, INITIAL_SPEED - (_foodEaten * SPEED_INCREMENT));

      _foods.erase(_foods.begin() + i);
      return true;
    }
  }

  return false;
}

void Snake::handleFoodSpawnAfterEating() {
  if (!_foods.empty()) {
    return;
  }

  spawnFood();

  if ((rand() % 100) < static_cast<int>(BONUS_FOOD_SPAWN_CHANCE * 100)) {
    spawnBonusFood();
  }
}

void Snake::trimSnakeTail() {
  if (_snake.empty()) {
    return;
  }

  _snake.pop_back();
  if (!_snake.empty()) {
    _snake.back().type = EntityTypeSnake::SNAKE_TAIL;
  }
}

void Snake::moveSnake(float deltaTime) {
  if (!canMoveThisFrame(deltaTime)) {
    return;
  }

  Position nextHead;
  if (!prepareNextMove(nextHead)) {
    return;
  }

  if (eatFoodAt(nextHead)) {
    handleFoodSpawnAfterEating();
  } else {
    trimSnakeTail();
  }

  if (_isBoosting) {
    _isBoosting = false;
  }
}
