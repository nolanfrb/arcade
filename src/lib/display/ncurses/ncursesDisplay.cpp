/*
** EPITECH PROJECT, 2026
** arcade
** File description:
** ncursesDisplay
*/

#include "ncursesDisplay.hpp"
#include <ncurses.h>
#include <cstddef>
#include <optional>
#include <string>
#include <vector>
#include "../../../shared/Entity.hpp"
#include "../../../shared/Input.hpp"
#include "../../../shared/Text.hpp"
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

void NcursesDisplay::init() { Ncurses::init(); }

void NcursesDisplay::stop() { Ncurses::stop(); }

int NcursesDisplay::readPendingInput() {
  if (_pendingInput.has_value()) {
    return *_pendingInput;
  }
  const int input = Ncurses::readInput();
  if (input != ERR) {
    _pendingInput = input;
    _eventConsumed = false;
    _textConsumed = false;
  }
  return input;
}

void NcursesDisplay::clearPendingInput() {
  if (_eventConsumed && _textConsumed) {
    _pendingInput.reset();
    _eventConsumed = false;
    _textConsumed = false;
  }
}

Input NcursesDisplay::getEvent() {
  if (_eventConsumed && _pendingInput.has_value()) {
    clearPendingInput();
  }
  const int input = readPendingInput();
  if (input == ERR) {
    return Input::NONE;
  }
  if (_eventConsumed) {
    return Input::NONE;
  }
  _eventConsumed = true;
  clearPendingInput();

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
    case 'r':
      return Input::RESTART;
    case 'g':
      return Input::NEXTGAME;
    case 'h':
      return Input::NEXTDISPLAY;
    default:
      return Input::NONE;
  }
}

void NcursesDisplay::clear() { Ncurses::clear(); }

void NcursesDisplay::drawEntity(const std::vector<Entity>& entities) {
  for (const auto& entity : entities) {
    Ncurses::putItem(static_cast<int>(entity.position.y),
                     static_cast<int>(entity.position.x),
                     entity.type.asciiChar);
  }
}

void NcursesDisplay::drawText(const std::vector<Text>& texts) {
  for (const auto& text : texts) {
    for (std::size_t i = 0; i < text.content.size(); ++i) {
      Ncurses::putItem(
          static_cast<int>(text.position.y),
          static_cast<int>(text.position.x + static_cast<float>(i)),
          text.content[i]);
    }
  }
}

std::optional<std::string> NcursesDisplay::getTextInput() {
  if (_textConsumed && _pendingInput.has_value()) {
    clearPendingInput();
  }

  const int input = readPendingInput();
  if (input == ERR) {
    return std::nullopt;
  }

  if (_textConsumed) {
    return _textInputBuffer;
  }

  _textConsumed = true;

  if (input == KEY_BACKSPACE || input == BACKSPACE_KEY ||
      input == CTRL_BACKSPACE_KEY) {
    if (!_textInputBuffer.empty()) {
      _textInputBuffer.pop_back();
    }
    clearPendingInput();
    return _textInputBuffer;
  }
  if (input >= PRINTABLE_ASCII_MIN && input <= PRINTABLE_ASCII_MAX) {
    _textInputBuffer.push_back(static_cast<char>(input));
  }
  clearPendingInput();
  return _textInputBuffer;
}

void NcursesDisplay::display() { refresh(); }

extern "C" gsl::owner<IDisplay*> createDisplay() {
  return new NcursesDisplay();
}
