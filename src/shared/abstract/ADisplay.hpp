/*
** EPITECH PROJECT, 2026
** arcade
** File description:
** ADisplay
*/

#pragma once

#include <string>
#include <vector>
#include <optional>
#include "../Entity.hpp"
#include "../Input.hpp"
#include "../Text.hpp"
#include "../interface/IDisplay.hpp"

class ADisplay : public IDisplay {
 public:
  ADisplay() = default;
  ~ADisplay() override = default;

  ADisplay(const ADisplay&) = default;
  ADisplay& operator=(const ADisplay&) = default;
  ADisplay(ADisplay&&) = default;
  ADisplay& operator=(ADisplay&&) = default;

  void init() override = 0;
  void stop() override = 0;

  std::string getName() override { return _name; }
  Input getEvent() override = 0;
  std::optional<std::string> getTextInput() override = 0;

  void clear() override = 0;
  void drawEntity(const std::vector<Entity>& entities) override = 0;
  void drawText(const std::vector<Text>& texts) override = 0;

  void display() override = 0;

 protected:
  void setName(const std::string& name) { _name = name; }

 private:
  std::string _name;
};
