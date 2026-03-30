/*
** EPITECH PROJECT, 2026
** arcade
** File description:
** Scene - fluent API for building Entity/Text collections
*/

#pragma once

#include <array>
#include <cstdint>
#include <string>
#include <utility>
#include <vector>
#include "../../shared/Entity.hpp"
#include "../../shared/Text.hpp"

using Color = std::array<uint8_t, 4>;

namespace Colors {
constexpr Color WHITE = {255, 255, 255, 255};
constexpr Color YELLOW = {255, 255, 0, 255};
constexpr Color RED = {255, 0, 0, 255};
constexpr Color GREEN = {0, 255, 0, 255};
constexpr Color BLUE = {0, 0, 255, 255};
constexpr Color BLACK = {0, 0, 0, 255};
}  // namespace Colors

class Scene {
 public:
  Scene& label(const std::string& content, Position pos,
               Color color = Colors::WHITE, int fontSize = 16) {
    Text text;
    text.content = content;
    text.position = pos;
    text.color = color;
    text.fontSize = fontSize;
    _texts.push_back(text);
    return *this;
  }

  Scene& box(Position pos, float width, float height,
             Color color = Colors::WHITE, char asciiChar = '#') {
    Entity e;
    e.position = pos;
    e.type = {.type = Shape::RECTANGLE,
              .width = width,
              .height = height,
              .asciiChar = asciiChar,
              .color = color};
    _entities.push_back(e);
    return *this;
  }

  Scene& sprite(Position pos, const std::string& spritePath, float width = 1,
                float height = 1, char asciiChar = '?') {
    Entity e;
    e.position = pos;
    e.type = {.type = Shape::SPRITE,
              .width = width,
              .height = height,
              .asciiChar = asciiChar,
              .color = Colors::WHITE,
              .spritePath = spritePath};
    _entities.push_back(e);
    return *this;
  }

  Scene& circle(Position pos, float radius, Color color = Colors::WHITE,
                char asciiChar = 'O') {
    Entity e;
    e.position = pos;
    e.type = {.type = Shape::CIRCLE,
              .width = radius * 2,
              .height = radius * 2,
              .asciiChar = asciiChar,
              .color = color};
    _entities.push_back(e);
    return *this;
  }

  Scene& list(const std::vector<std::string>& items, Position pos,
              std::size_t selectedIndex, float spacing = 2.0F,
              Color color = Colors::WHITE, Color selectedColor = Colors::YELLOW,
              const std::string& prefix = "  ",
              const std::string& selectedPrefix = "> ") {
    for (std::size_t i = 0; i < items.size(); i++) {
      bool sel = (i == selectedIndex);
      Text text;
      text.content = (sel ? selectedPrefix : prefix) + items[i];
      text.position = {.x = pos.x, .y = pos.y + (static_cast<float>(i) * spacing)};
      text.color = sel ? selectedColor : color;
      _texts.push_back(text);
    }
    return *this;
  }

  void clear() {
    _entities.clear();
    _texts.clear();
  }

  [[nodiscard]] const std::vector<Entity>& entities() const {
    return _entities;
  }
  [[nodiscard]] const std::vector<Text>& texts() const { return _texts; }

 private:
  std::vector<Entity> _entities;
  std::vector<Text> _texts;
};
