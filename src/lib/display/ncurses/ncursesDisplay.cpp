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
#include <optional>
#include <string>
#include <vector>
#include "../../../shared/Entity.hpp"
#include "../../../shared/Input.hpp"
#include "../../../shared/Text.hpp"
#include "../../../shared/abstract/ADisplay.hpp"
#include "ncurses/ncurses.hpp"

namespace {
constexpr int ESC_KEY = 27;
constexpr int BACKSPACE_KEY = 127;
constexpr int CTRL_BACKSPACE_KEY = 8;
constexpr int PRINTABLE_ASCII_MIN = 32;
constexpr int PRINTABLE_ASCII_MAX = 126;
}  // namespace

namespace gsl {
template <typename T>
using owner = T;
}  // namespace gsl

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
    case ESC_KEY:
      return Input::MENU;
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

std::optional<std::string> ncursesDisplay::getTextInput() {
  const int input = ncurses::readInput();

  if (input == ERR) {
    return std::nullopt;
  }
  if (input == KEY_BACKSPACE || input == BACKSPACE_KEY ||
      input == CTRL_BACKSPACE_KEY) {
    if (!_textInputBuffer.empty()) {
      _textInputBuffer.pop_back();
    }
    return _textInputBuffer;
  }
  if (input >= PRINTABLE_ASCII_MIN && input <= PRINTABLE_ASCII_MAX) {
    _textInputBuffer.push_back(static_cast<char>(input));
  }
  return _textInputBuffer;
}

void ncursesDisplay::display() { refresh(); }

extern "C" gsl::owner<ADisplay*> createDisplay() {
  return new ncursesDisplay();
}
