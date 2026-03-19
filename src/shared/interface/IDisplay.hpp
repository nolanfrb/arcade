/*
** EPITECH PROJECT, 2026
** Arcade
** File description:
** IDisplay
*/

#pragma once

#include <string>
#include <vector>
#include "../Entity.hpp"
#include "../Input.hpp"
#include "../Text.hpp"

class IDisplay {
 public:
  virtual ~IDisplay() = default;

  virtual void init() = 0;
  virtual void stop() = 0;
  virtual std::string getName() = 0;

  virtual Input getEvent() = 0;

  virtual void clear() = 0;
  virtual void drawEntity(const std::vector<Entity>& entities) = 0;
  virtual void drawText(const std::vector<Text>& texts) = 0;

  virtual void display() = 0;

  IDisplay(const IDisplay&) = default;
  IDisplay& operator=(const IDisplay&) = default;
  IDisplay(IDisplay&&) = default;
  IDisplay& operator=(IDisplay&&) = default;
};
