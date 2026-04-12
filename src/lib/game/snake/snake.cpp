/*
** EPITECH PROJECT, 2026
** arcade
** File description:
** snake
*/

#include "snake.hpp"
#include <cstdlib>

namespace gsl {
template <typename T>
using owner = T;
}  // namespace gsl

extern "C" gsl::owner<IGame*> createGame() { return new Snake(); }
extern "C" void destroyGame(gsl::owner<IGame*> game) { delete game; }

void Snake::init() {
  srand(static_cast<unsigned>(time(nullptr)));
  setName("Snake");
  clearEntities();
  clearTexts();
  _snake.clear();
  _foods.clear();
  _currentDirection = Input::RIGHT;
  _nextDirection = Input::RIGHT;
  _moveTimer = 0.F;
  _currentSpeed = INITIAL_SPEED;
  _isBoosting = false;
  _boostCooldown = 0.F;
  _foodEaten = 0;
  setScore(0);
  setIsGameOver(false);
  createBorders();
  const float startX = static_cast<float>(PLAY_AREA_X + PLAY_AREA_WIDTH / 2);
  const float startY = static_cast<float>(PLAY_AREA_Y + PLAY_AREA_HEIGHT / 2);
  _snake.push_back(
      {Position{startX, startY}, EntityTypeSnake::SNAKE_HEAD_RIGHT});
  _snake.push_back(
      {Position{startX - 1.F, startY}, EntityTypeSnake::SNAKE_BODY});
  _snake.push_back(
      {Position{startX - 2.F, startY}, EntityTypeSnake::SNAKE_BODY});
  _snake.push_back(
      {Position{startX - 3.F, startY}, EntityTypeSnake::SNAKE_TAIL});
  spawnFood();
  updateEntities();
}

void Snake::stop() { setIsGameOver(true); }

void Snake::restart() { init(); }

void Snake::handleInput(Input input) {
  if (input == Input::NONE) {
    return;
  }

  if (input == Input::UP && _currentDirection != Input::DOWN) {
    _nextDirection = Input::UP;
  } else if (input == Input::DOWN && _currentDirection != Input::UP) {
    _nextDirection = Input::DOWN;
  } else if (input == Input::LEFT && _currentDirection != Input::RIGHT) {
    _nextDirection = Input::LEFT;
  } else if (input == Input::RIGHT && _currentDirection != Input::LEFT) {
    _nextDirection = Input::RIGHT;
  } else if (input == Input::ACTION) {
    if (_boostCooldown <= 0.F) {
      _isBoosting = true;
      _boostCooldown = BOOST_COOLDOWN;
    }
  }
}

void Snake::update(Input input, float deltaTime) {
  handleInput(input);

  if (!isGameOver()) {
    moveSnake(deltaTime);
    updateBonusFood(deltaTime);
  }

  updateEntities();
  updateGameOverText();

  _boostCooldown -= deltaTime;
}
