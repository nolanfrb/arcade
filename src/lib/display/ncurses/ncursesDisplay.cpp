/*
** EPITECH PROJECT, 2026
** arcade
** File description:
** ncursesDisplay
*/

#include "ncursesDisplay.hpp"
#include <ncurses.h>
#include <cstddef>
#include <cstdint>
#include <vector>
#include "../../../shared/Entity.hpp"
#include "../../../shared/Input.hpp"
#include "../../../shared/Text.hpp"
#include "ncurses/ncurses.hpp"

void ncursesDisplay::init() { ncurses::init(); }

void ncursesDisplay::stop() { ncurses::stop(); }

Input ncursesDisplay::getEvent() {
  const int input = ncurses::readInput();
  switch (input) {
    case KEY_UP:
      return Input::UP;
    case KEY_DOWN:
      return Input::DOWN;
    case KEY_LEFT:
      return Input::LEFT;
    case KEY_RIGHT:
      return Input::RIGHT;
    case ' ':
      return Input::ACTION;
    case 'q':
      return Input::EXIT;
    default:
      return Input::NONE;
  }
}

void ncursesDisplay::clear() { ncurses::clear(); }

void ncursesDisplay::drawEntity(const std::vector<Entity>& entities) {
  for (const auto& entity : entities) {
    ncurses::putItem(static_cast<int8_t>(entity.position.y),
                     static_cast<int8_t>(entity.position.x), entity.asciiChar);
  }
}

void ncursesDisplay::drawText(const std::vector<Text>& texts) {
  for (const auto& text : texts) {
    for (std::size_t i = 0; i < text.content.size(); ++i) {
      ncurses::putItem(
          static_cast<int8_t>(text.position.y),
          static_cast<int8_t>(text.position.x + static_cast<float>(i)),
          text.content[i]);
    }
  }
}

void ncursesDisplay::display() { refresh(); }
