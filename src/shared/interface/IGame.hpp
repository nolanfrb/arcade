/*
** EPITECH PROJECT, 2026
** Arcade
** File description:
** IGame
*/

#pragma once

#include <string>
#include <vector>
#include "../Entity.hpp"
#include "../Input.hpp"
#include "../Text.hpp"
#include "IGameContext.hpp"

class IGame {
 public:
  virtual ~IGame() = default;
  IGame() = default;

  IGame(const IGame&) = default;
  IGame& operator=(const IGame&) = default;
  IGame(IGame&&) = default;
  IGame& operator=(IGame&&) = default;

  virtual void init() = 0;
  virtual void stop() = 0;
  virtual void restart() = 0;

  virtual void update(Input input, float deltaTime) = 0;
  virtual void setContext(IGameContext& ctx) = 0;

  virtual std::vector<Entity> getEntity() = 0;
  virtual std::vector<Text> getText() = 0;

  virtual int getScore() = 0;
  virtual bool isGameOver() = 0;
  virtual std::string getName() = 0;
};
