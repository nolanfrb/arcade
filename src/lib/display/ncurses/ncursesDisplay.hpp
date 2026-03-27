/*
** EPITECH PROJECT, 2026
** arcade
** File description:
** ncursesDisplay
*/

#pragma once

#include <optional>
#include <string>
#include <vector>
#include "../../../shared/Entity.hpp"
#include "../../../shared/Input.hpp"
#include "../../../shared/Text.hpp"
#include "../../../shared/abstract/ADisplay.hpp"

class NcursesDisplay : public ADisplay {
 public:
  NcursesDisplay() { setName("ncurses"); }
  ~NcursesDisplay() override = default;
  NcursesDisplay(const NcursesDisplay&) = default;
  NcursesDisplay& operator=(const NcursesDisplay&) = default;
  NcursesDisplay(NcursesDisplay&&) = default;
  NcursesDisplay& operator=(NcursesDisplay&&) = default;

  void init() override;
  void stop() override;

  Input getEvent() override;
  std::optional<std::string> getTextInput() override;

  void clear() override;
  void drawEntity(const std::vector<Entity>& entities) override;
  void drawText(const std::vector<Text>& texts) override;

  void display() override;

 private:
  int readPendingInput();
  void clearPendingInput();

  std::optional<int> _pendingInput;
  bool _eventConsumed{false};
  bool _textConsumed{false};
  std::string _textInputBuffer;
};
