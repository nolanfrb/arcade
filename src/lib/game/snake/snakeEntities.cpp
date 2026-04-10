/*
** EPITECH PROJECT, 2026
** arcade
** File description:
** snake entities
*/

#include <sstream>
#include "EntityFactory.hpp"
#include "snake.hpp"

namespace {
// NOLINTNEXTLINE(bugprone-easily-swappable-parameters)
float getCenteredTextPosX(const std::string& text, int fontSize,
                          float screenWidthPx, float tilePx) {
  const float textWidthPx =
      static_cast<float>(text.size()) * static_cast<float>(fontSize);
  return (screenWidthPx - textWidthPx) / (2.F * tilePx);
}
}  // namespace

Position Snake::toScreenPos(const Position& pos) {
  return Position{.x = pos.x + MAP_OFFSET_X, .y = pos.y + MAP_OFFSET_Y};
}

void Snake::createBorders() {
  for (int col = 0; col < MAP_WIDTH; ++col) {
    for (int row = 0; row < MAP_HEIGHT; ++row) {
      if (col == 0 || col == MAP_WIDTH - 1 || row == 0 ||
          row == MAP_HEIGHT - 1) {
        addEntity(EntityFactory::createTile(
            toScreenPos(Position{.x = static_cast<float>(col),
                                 .y = static_cast<float>(row)}),
            "assets/snake/border.png", WHITE, '#'));
      }
    }
  }
}

void Snake::displayHUD() {
  std::ostringstream scoreStream;
  scoreStream << "Score: " << getScore();

  Text scoreText;
  scoreText.content = scoreStream.str();
  scoreText.position = Position{.x = MAP_OFFSET_X, .y = HUD_Y};
  scoreText.fontSize = 14;
  scoreText.color = WHITE;
  scoreText.fontPath = "";
  addText(scoreText);
}

void Snake::updateGameOverText() {
  if (!isGameOver()) {
    return;
  }

  constexpr float SCREEN_WIDTH_PX = static_cast<float>(SCREEN_TILES_X) * 20.F;
  constexpr float SCREEN_CENTER_Y = static_cast<float>(SCREEN_TILES_Y) / 2.F;
  constexpr float TILE_PX = 20.F;

  clearTexts();

  std::ostringstream scoreStream;
  scoreStream << "GAME OVER! Final Score: " << getScore();

  Text gameOverText;
  gameOverText.content = scoreStream.str();
  gameOverText.fontSize = 16;
  gameOverText.position = Position{
      .x = getCenteredTextPosX(gameOverText.content, gameOverText.fontSize,
                               SCREEN_WIDTH_PX, TILE_PX),
      .y = SCREEN_CENTER_Y - 1.F};
  gameOverText.color = RED;
  gameOverText.fontPath = "";
  addText(gameOverText);

  Text restartText;
  restartText.content = "Press R to Restart or ESC for Menu";
  restartText.fontSize = 14;
  restartText.position = Position{
      .x = getCenteredTextPosX(restartText.content, restartText.fontSize,
                               SCREEN_WIDTH_PX, TILE_PX),
      .y = SCREEN_CENTER_Y + 1.F};
  restartText.color = WHITE;
  restartText.fontPath = "";
  addText(restartText);
}

void Snake::addSnakeEntities() {
  for (const auto& segment : _snake) {
    Entity entity = EntityFactory::createSnakeEntity(segment);
    entity.position = toScreenPos(entity.position);
    addEntity(entity);
  }
}

void Snake::addFoodEntities() {
  for (const auto& food : _foods) {
    Entity entity = EntityFactory::createFoodEntity(food);
    entity.position = toScreenPos(entity.position);
    addEntity(entity);
  }
}

void Snake::updateEntities() {
  clearEntities();
  clearTexts();
  createBorders();
  addSnakeEntities();
  addFoodEntities();
  if (!isGameOver()) {
    displayHUD();
  }
}
