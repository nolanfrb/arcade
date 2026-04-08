/*
** EPITECH PROJECT, 2026
** arcade
** File description:
** snake entities
*/

#include <sstream>
#include "EntityFactory.hpp"
#include "snake.hpp"

void Snake::createBorders() {
  for (int col = 0; col < MAP_WIDTH; ++col) {
    for (int row = 0; row < MAP_HEIGHT; ++row) {
      if (col == 0 || col == MAP_WIDTH - 1 || row == 0 ||
          row == MAP_HEIGHT - 1) {
        addEntity(
            EntityFactory::createTile(Position{.x = static_cast<float>(col),
                                               .y = static_cast<float>(row)},
                                      "assets/snake/border.png", WHITE, '#'));
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
  addText(EntityFactory::createText(scoreStream.str(),
                                    Position{.x = 5.F, .y = 10.F}, RED));
  addText(EntityFactory::createText("Press R to Restart or ESC for Menu",
                                    Position{.x = 5.F, .y = 12.F}, WHITE));
}

void Snake::addSnakeEntities() {
  for (const auto& segment : _snake) {
    addEntity(EntityFactory::createSnakeEntity(segment));
  }
}

void Snake::addFoodEntities() {
  for (const auto& food : _foods) {
    addEntity(EntityFactory::createFoodEntity(food));
  }
}

void Snake::updateEntities() {
  clearEntities();
  createBorders();
  addSnakeEntities();
  addFoodEntities();
}
