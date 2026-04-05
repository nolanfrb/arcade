/*
** EPITECH PROJECT, 2026
** arcade
** File description:
** snake
*/

#include "snake.hpp"
#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <sstream>

extern "C" IGame* createGame() { return new Snake(); }

void Snake::createBorders() {
  for (int x = 0; x < MAP_WIDTH; ++x) {
    for (int y = 0; y < MAP_HEIGHT; ++y) {
      if (x == 0 || x == MAP_WIDTH - 1 || y == 0 || y == MAP_HEIGHT - 1) {
        Entity border;
        border.position =
            Position{static_cast<float>(x), static_cast<float>(y)};
        border.type.type = Shape::RECTANGLE;
        border.type.width = TILE_SIZE;
        border.type.height = TILE_SIZE;
        border.type.color = WHITE;
        border.type.asciiChar = '#';
        border.type.spritePath = "assets/snake/border.png";
        border.type.isTextInput = false;
        addEntity(border);
      }
    }
  }
}

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
  if (input == Input::NONE) return;

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

void Snake::moveSnake(float deltaTime) {
  _moveTimer += deltaTime;

  float speed = _isBoosting ? BOOST_SPEED : _currentSpeed;
  if (_moveTimer >= speed) {
    _moveTimer = 0.F;
    _currentDirection = _nextDirection;

    Position nextHead = getNextHeadPosition();

    if (isPositionOccupied(nextHead, true)) {
      setIsGameOver(true);
      return;
    }

    _snake.front().type = EntityTypeSnake::SNAKE_BODY;
    _snake.push_front({nextHead, getHeadTypeForDirection()});

    bool foodEaten = false;
    for (size_t i = 0; i < _foods.size(); ++i) {
      if (std::abs(_foods[i].position.x - nextHead.x) < 0.1F &&
          std::abs(_foods[i].position.y - nextHead.y) < 0.1F) {
        int score = _foods[i].isBonus ? BONUS_FOOD_SCORE : NORMAL_FOOD_SCORE;
        setScore(getScore() + score);
        _foodEaten++;

        _currentSpeed =
            std::max(MAX_SPEED, INITIAL_SPEED - (_foodEaten * SPEED_INCREMENT));

        _foods.erase(_foods.begin() + i);
        foodEaten = true;
        break;
      }
    }

    if (foodEaten) {
      if (_foods.empty()) {
        spawnFood();

        if ((rand() % 100) < static_cast<int>(BONUS_FOOD_SPAWN_CHANCE * 100)) {
          spawnBonusFood();
        }
      }
    } else {
      if (!_snake.empty()) {
        _snake.pop_back();
        if (!_snake.empty()) {
          _snake.back().type = EntityTypeSnake::SNAKE_TAIL;
        }
      }
    }

    if (_isBoosting) {
      _isBoosting = false;
    }
  }
}

void Snake::spawnFood() {
  Position newFood;
  bool valid = false;
  const int maxAttempts = PLAY_AREA_WIDTH * PLAY_AREA_HEIGHT;

  for (int attempt = 0; attempt < maxAttempts && !valid; ++attempt) {
    newFood.x = static_cast<float>(PLAY_AREA_X + (rand() % PLAY_AREA_WIDTH));
    newFood.y = static_cast<float>(PLAY_AREA_Y + (rand() % PLAY_AREA_HEIGHT));
    valid = !isPositionOccupied(newFood, true);
  }

  if (!valid) {
    return;
  }

  _foods.push_back({newFood, false, 0.F});
}

void Snake::spawnBonusFood() {
  Position newFood;
  bool valid = false;
  const int maxAttempts = PLAY_AREA_WIDTH * PLAY_AREA_HEIGHT;
  const int maxAttempts = PLAY_AREA_WIDTH * PLAY_AREA_HEIGHT;

  for (int attempt = 0; attempt < maxAttempts && !valid; ++attempt) {
    newFood.x = static_cast<float>(PLAY_AREA_X + (rand() % PLAY_AREA_WIDTH));
    newFood.y = static_cast<float>(PLAY_AREA_Y + (rand() % PLAY_AREA_HEIGHT));
    valid = !isPositionOccupied(newFood, true);
  }

  if (!valid) {
    return;
  }
  if (!valid) {
    return;
  }
  _foods.push_back({newFood, true, BONUS_FOOD_DURATION});
}

void Snake::updateBonusFood(float deltaTime) {
  for (auto it = _foods.begin(); it != _foods.end();) {
    if (it->isBonus) {
      it->bonusTimer -= deltaTime;
      if (it->bonusTimer <= 0.F) {
        it = _foods.erase(it);
        if (_foods.empty()) {
          spawnFood();
        }
      } else {
        ++it;
      }
    } else {
      ++it;
    }
  }
}

void Snake::updateGameOverText() {
  if (!isGameOver()) {
    return;
  }

  clearTexts();
  std::ostringstream scoreStream;
  scoreStream << "GAME OVER! Final Score: " << getScore();
  Text gameOverText;
  gameOverText.content = scoreStream.str();
  gameOverText.position = Position{5.F, 10.F};
  gameOverText.color = RED;
  gameOverText.fontPath = "";
  addText(gameOverText);

  Text restartText;
  restartText.content = "Press R to Restart or G for Menu";
  restartText.position = Position{5.F, 12.F};
  restartText.color = WHITE;
  restartText.fontPath = "";
  addText(restartText);
}

void Snake::updateEntities() {
  clearEntities();

  createBorders();

  for (const auto& segment : _snake) {
    Entity entity;
    entity.position = segment.position;

    if (segment.type == EntityTypeSnake::SNAKE_HEAD_UP) {
      entity.type.type = Shape::SPRITE;
      entity.type.spritePath = "assets/snake/snake_head_up.png";
      entity.type.width = TILE_SIZE;
      entity.type.height = TILE_SIZE;
      entity.type.color = DARK_GREEN;
      entity.type.asciiChar = '^';
    } else if (segment.type == EntityTypeSnake::SNAKE_HEAD_DOWN) {
      entity.type.type = Shape::SPRITE;
      entity.type.spritePath = "assets/snake/snake_head_down.png";
      entity.type.width = TILE_SIZE;
      entity.type.height = TILE_SIZE;
      entity.type.color = DARK_GREEN;
      entity.type.asciiChar = 'v';
    } else if (segment.type == EntityTypeSnake::SNAKE_HEAD_LEFT) {
      entity.type.type = Shape::SPRITE;
      entity.type.spritePath = "assets/snake/snake_head_left.png";
      entity.type.width = TILE_SIZE;
      entity.type.height = TILE_SIZE;
      entity.type.color = DARK_GREEN;
      entity.type.asciiChar = '<';
    } else if (segment.type == EntityTypeSnake::SNAKE_HEAD_RIGHT) {
      entity.type.type = Shape::SPRITE;
      entity.type.spritePath = "assets/snake/snake_head_right.png";
      entity.type.width = TILE_SIZE;
      entity.type.height = TILE_SIZE;
      entity.type.color = DARK_GREEN;
      entity.type.asciiChar = '>';
    } else if (segment.type == EntityTypeSnake::SNAKE_BODY) {
      entity.type.type = Shape::SPRITE;
      entity.type.spritePath = "assets/snake/snake_body.png";
      entity.type.width = TILE_SIZE;
      entity.type.height = TILE_SIZE;
      entity.type.color = GREEN;
      entity.type.asciiChar = 'o';
    } else if (segment.type == EntityTypeSnake::SNAKE_TAIL) {
      entity.type.type = Shape::SPRITE;
      entity.type.spritePath = "assets/snake/snake_tail.png";
      entity.type.width = TILE_SIZE;
      entity.type.height = TILE_SIZE;
      entity.type.color = GREEN;
      entity.type.asciiChar = '~';
    }

    entity.type.isTextInput = false;
    addEntity(entity);
  }

  for (const auto& food : _foods) {
    Entity entity;
    entity.position = food.position;

    if (food.isBonus) {
      entity.type.type = Shape::SPRITE;
      entity.type.spritePath = "assets/snake/golden_apple.png";
      entity.type.color = GOLD;
      entity.type.asciiChar = '*';
    } else {
      entity.type.type = Shape::SPRITE;
      entity.type.spritePath = "assets/snake/apple.png";
      entity.type.color = RED;
      entity.type.asciiChar = 'X';
    }

    entity.type.width = TILE_SIZE;
    entity.type.height = TILE_SIZE;
    entity.type.isTextInput = false;
    addEntity(entity);
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
