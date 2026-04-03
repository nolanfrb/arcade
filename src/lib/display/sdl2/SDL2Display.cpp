/*
** EPITECH PROJECT, 2026
** Arcade
** File description:
** SDL2Display
*/

#include "SDL2Display.hpp"
#include <SDL2/SDL_events.h>
#include <optional>
#include <string>
#include "../../../shared/Input.hpp"
#include "../../../shared/Text.hpp"
#include "../../../shared/interface/IDisplay.hpp"

namespace gsl {
template <typename T>
using owner = T;
}  // namespace gsl

void SDL2Display::init() {
  _renderer.init(sdl2::WINDOW_WIDTH, sdl2::WINDOW_HEIGHT, sdl2::WINDOW_TITLE);
}

void SDL2Display::stop() { _renderer.close(); }

void SDL2Display::pollEvents() {
  _pendingEvent.reset();
  _pendingText.reset();
  _eventConsumed = false;
  _textConsumed = false;
  SDL_Event event{};
  while (_renderer.pollEvent(event)) {
    if (event.type == SDL_QUIT) {
      _pendingEvent = Input::EXIT;
      return;
    }
    Input mapped = SDL2Renderer::mapEvent(event);
    if (mapped != Input::NONE) {
      _pendingEvent = mapped;
      return;
    }
    char textChar = SDL2Renderer::mapTextEvent(event);
    if (textChar != '\0') {
      _pendingText = textChar;
      return;
    }
  }
}

Input SDL2Display::getEvent() {
  pollEvents();
  if (_eventConsumed || !_pendingEvent.has_value()) {
    return Input::NONE;
  }
  _eventConsumed = true;
  const Input result = _pendingEvent.value();
  _pendingEvent.reset();
  _pendingText.reset();
  _textConsumed = true;
  return result;
}

std::optional<std::string> SDL2Display::getTextInput() {
  pollEvents();
  if (_textConsumed) {
    return _textInputBuffer;
  }
  if (!_pendingText.has_value()) {
    return std::nullopt;
  }
  _textConsumed = true;
  const char input = _pendingText.value();
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

void SDL2Display::clear() { _renderer.clear(); }

void SDL2Display::drawEntity(const std::vector<Entity>& entities) {
  for (const auto& entity : entities) {
    _renderer.drawEntity(entity);
  }
}

void SDL2Display::drawText(const std::vector<Text>& texts) {
  for (const auto& text : texts) {
    _renderer.drawText(text);
  }
}

void SDL2Display::display() { _renderer.display(); }

extern "C" gsl::owner<IDisplay*> createDisplay() { return new SDL2Display(); }
