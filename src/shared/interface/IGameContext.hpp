/*
** EPITECH PROJECT, 2026
** arcade
** File description:
** IGameContext
*/

#pragma once

#include <string>
#include <vector>

class IGameContext {
 public:
  virtual ~IGameContext() = default;
  IGameContext() = default;

  IGameContext(const IGameContext&) = default;
  IGameContext& operator=(const IGameContext&) = default;
  IGameContext(IGameContext&&) = default;
  IGameContext& operator=(IGameContext&&) = default;

  [[nodiscard]] virtual std::vector<std::string> getGameList() const = 0;
  [[nodiscard]] virtual std::vector<std::string> getDisplayList() const = 0;
  virtual void loadGame(const std::string& path) = 0;
  virtual void loadDisplay(const std::string& path) = 0;
};
