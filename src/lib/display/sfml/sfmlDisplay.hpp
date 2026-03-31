/*
** EPITECH PROJECT, 2026
** Arcade
** File description:
** sfmlDisplay
*/

#pragma once

#include <optional>
#include <string>
#include "../../../shared/Input.hpp"
#include "../../../shared/abstract/ADisplay.hpp"
#include "wrapper/sfmlRenderer.hpp"

class SfmlDisplay : public ADisplay {
 public:
  SfmlDisplay() { setName("sfml"); }
  ~SfmlDisplay() override = default;
  SfmlDisplay(const SfmlDisplay&) = delete;
  SfmlDisplay& operator=(const SfmlDisplay&) = delete;
  SfmlDisplay(SfmlDisplay&&) = delete;
  SfmlDisplay& operator=(SfmlDisplay&&) = delete;

  void init() override;
  void stop() override;

  Input getEvent() override;
  std::optional<std::string> getTextInput() override;

  void clear() override;
  void drawEntity(const std::vector<Entity>& entities) override;
  void drawText(const std::vector<Text>& texts) override;

  void display() override;

 private:
  SfmlRenderer _renderer;
  std::optional<Input> _pendingEvent;
  std::optional<char> _pendingText;
  std::string _textInputBuffer;
  bool _eventConsumed{false};
  bool _textConsumed{false};

  void pollEvents();
};
