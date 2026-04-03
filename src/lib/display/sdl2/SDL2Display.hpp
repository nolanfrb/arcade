/*
** EPITECH PROJECT, 2026
** Arcade
** File description:
** SDL2Display
*/

#pragma once

#include <optional>
#include <string>

#include "../../../shared/Input.hpp"
#include "../../../shared/Text.hpp"
#include "../../../shared/abstract/ADisplay.hpp"
#include "wrapper/SDL2Renderer.hpp"

class SDL2Display : public ADisplay {
 public:
  SDL2Display() { setName("SDL2"); }
  ~SDL2Display() override = default;

  SDL2Display(const SDL2Display&) = delete;
  SDL2Display& operator=(const SDL2Display&) = delete;
  SDL2Display(SDL2Display&&) = delete;
  SDL2Display& operator=(SDL2Display&&) = delete;

  void init() override;
  void stop() override;

  Input getEvent() override;
  std::optional<std::string> getTextInput() override;

  void clear() override;
  void drawEntity(const std::vector<Entity>& entities) override;
  void drawText(const std::vector<Text>& texts) override;

  void display() override;

 private:
  SDL2Renderer _renderer;
  std::optional<Input> _pendingEvent;
  std::optional<char> _pendingText;
  std::string _textInputBuffer;
  bool _eventConsumed{false};
  bool _textConsumed{false};

  void pollEvents();
};
