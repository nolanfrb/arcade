/*
** EPITECH PROJECT, 2026
** Arcade
** File description:
** sfmlDisplay
*/

#include "sfmlDisplay.hpp"
#include <SFML/Window/Event.hpp>
#include <optional>
#include <string>
#include "../../../shared/Input.hpp"
#include "constants/sfmlConstants.hpp"  // NOLINT(misc-include-cleaner)
namespace gsl {
template <typename T>
using owner = T;
}  // namespace gsl

void SfmlDisplay::init() {
  _renderer.init(sfml::WINDOW_WIDTH, sfml::WINDOW_HEIGHT, sfml::WINDOW_TITLE);
}

void SfmlDisplay::stop() { _renderer.close(); }

void SfmlDisplay::pollEvents() {
  _pendingEvent.reset();
  _pendingText.reset();
  _eventConsumed = false;
  _textConsumed = false;
  sf::Event event{};
  while (_renderer.pollEvent(event)) {
    if (event.type == sf::Event::Closed) {
      _pendingEvent = Input::EXIT;
      return;
    }
    Input mapped = SfmlRenderer::mapEvent(event);
    if (mapped != Input::NONE) {
      _pendingEvent = mapped;
      return;
    }
    char textChar = SfmlRenderer::mapTextEvent(event);
    if (textChar != '\0') {
      _pendingText = textChar;
      return;
    }
  }
}

Input SfmlDisplay::getEvent() {
  pollEvents();
  if (_eventConsumed || !_pendingEvent.has_value()) {
    return Input::NONE;
  }
  _eventConsumed = true;
  const Input result{*_pendingEvent};  // NOLINT(cppcoreguidelines-init-variables)
  _pendingEvent.reset();
  _pendingText.reset();
  _textConsumed = true;
  return result;
}

std::optional<std::string> SfmlDisplay::getTextInput() {
  pollEvents();
  if (_textConsumed) {
    return _textInputBuffer;
  }
  if (!_pendingText.has_value()) {
    return std::nullopt;
  }
  _textConsumed = true;
  const char input{*_pendingText};  // NOLINT(cppcoreguidelines-init-variables)
  _pendingText.reset();
  _pendingEvent.reset();
  _eventConsumed = true;

  if (input == '\b') {
    if (!_textInputBuffer.empty()) {
      _textInputBuffer.pop_back();
    }
  } else {
    _textInputBuffer.push_back(input);
  }
  return _textInputBuffer;
}

void SfmlDisplay::clear() { _renderer.clear(); }

void SfmlDisplay::drawEntity(const std::vector<Entity>& entities) {
  for (const auto& entity : entities) {
    _renderer.drawEntity(entity);
  }
}

void SfmlDisplay::drawText(const std::vector<Text>& texts) {
  for (const auto& text : texts) {
    _renderer.drawText(text);
  }
}

void SfmlDisplay::display() { _renderer.display(); }

extern "C" gsl::owner<IDisplay*> createDisplay() { return new SfmlDisplay(); }
