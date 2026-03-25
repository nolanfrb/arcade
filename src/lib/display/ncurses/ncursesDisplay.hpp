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

class ncursesDisplay : public ADisplay {
 public:
  ncursesDisplay();
  ~ncursesDisplay() override;
  ncursesDisplay(const ncursesDisplay&) = default;
  ncursesDisplay& operator=(const ncursesDisplay&) = default;
  ncursesDisplay(ncursesDisplay&&) = default;
  ncursesDisplay& operator=(ncursesDisplay&&) = default;

  void init() override;
  void stop() override;

  Input getEvent() override;
  std::optional<std::string> getTextInput() override;

  void clear() override;
  void drawEntity(const std::vector<Entity>& entities) override;
  void drawText(const std::vector<Text>& texts) override;

  void display() override;

 private:
  std::string _textInputBuffer;
};
