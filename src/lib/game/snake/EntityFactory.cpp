/*
** EPITECH PROJECT, 2026
** arcade
** File description:
** EntityFactory - static utility methods for creating entities and texts
*/

#include "EntityFactory.hpp"

// NOLINTNEXTLINE(bugprone-easily-swappable-parameters)
Entity EntityFactory::createTile(const Position& pos,
                                 const std::string& spritePath,
                                 const std::array<std::uint8_t, 4>& color,
                                 char asciiChar) {
  Entity entity;
  entity.position = pos;
  entity.type.type = Shape::SPRITE;
  entity.type.spritePath = spritePath;
  entity.type.width = TILE_SIZE;
  entity.type.height = TILE_SIZE;
  entity.type.color = color;
  entity.type.asciiChar = asciiChar;
  entity.type.isTextInput = false;
  return entity;
}

EntityFactory::SnakeSegmentInfo EntityFactory::getSegmentInfo(
    EntityTypeSnake type) {
  switch (type) {
    case EntityTypeSnake::SNAKE_HEAD_UP:
      return {.spritePath = "assets/snake/snake_head_up.png",
              .color = DARK_GREEN,
              .asciiChar = '^'};
    case EntityTypeSnake::SNAKE_HEAD_DOWN:
      return {.spritePath = "assets/snake/snake_head_down.png",
              .color = DARK_GREEN,
              .asciiChar = 'v'};
    case EntityTypeSnake::SNAKE_HEAD_LEFT:
      return {.spritePath = "assets/snake/snake_head_left.png",
              .color = DARK_GREEN,
              .asciiChar = '<'};
    case EntityTypeSnake::SNAKE_HEAD_RIGHT:
      return {.spritePath = "assets/snake/snake_head_right.png",
              .color = DARK_GREEN,
              .asciiChar = '>'};
    case EntityTypeSnake::SNAKE_BODY:
      return {.spritePath = "assets/snake/snake_body.png",
              .color = GREEN,
              .asciiChar = 'o'};
    case EntityTypeSnake::SNAKE_TAIL:
      return {.spritePath = "assets/snake/snake_tail.png",
              .color = GREEN,
              .asciiChar = '~'};
    default:
      return {.spritePath = "assets/snake/snake_body.png",
              .color = GREEN,
              .asciiChar = '?'};
  }
}

Entity EntityFactory::createSnakeEntity(const SnakeSegment& segment) {
  const auto info = getSegmentInfo(segment.type);
  return createTile(segment.position, info.spritePath, info.color,
                    info.asciiChar);
}

Entity EntityFactory::createFoodEntity(const FoodItem& food) {
  if (food.isBonus) {
    return createTile(food.position, "assets/snake/golden_apple.png", GOLD,
                      '*');
  }
  return createTile(food.position, "assets/snake/apple.png", RED, 'X');
}

Text EntityFactory::createText(const std::string& content, const Position& pos,
                               const std::array<std::uint8_t, 4>& color) {
  Text text;
  text.content = content;
  text.position = pos;
  text.color = color;
  text.fontPath = "";
  return text;
}
