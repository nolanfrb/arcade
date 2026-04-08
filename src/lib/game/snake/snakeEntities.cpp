/*
** EPITECH PROJECT, 2026
** arcade
** File description:
** snake entities
*/

#include <sstream>
#include "snake.hpp"

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
  restartText.content = "Press R to Restart or ESC for Menu";
  restartText.position = Position{5.F, 12.F};
  restartText.color = WHITE;
  restartText.fontPath = "";
  addText(restartText);
}

Entity Snake::createSnakeEntity(const SnakeSegment& segment) {
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
  return entity;
}

Entity Snake::createFoodEntity(const FoodItem& food) {
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
  return entity;
}

void Snake::addSnakeEntities() {
  for (const auto& segment : _snake) {
    addEntity(createSnakeEntity(segment));
  }
}

void Snake::addFoodEntities() {
  for (const auto& food : _foods) {
    addEntity(createFoodEntity(food));
  }
}

void Snake::updateEntities() {
  clearEntities();
  createBorders();
  addSnakeEntities();
  addFoodEntities();
}
