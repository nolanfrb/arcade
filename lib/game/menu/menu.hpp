/*
** EPITECH PROJECT, 2026
** Arcade
** File description:
** menu
*/

#pragma once

#include "../../shared/abstract/AGame.hpp"

class Menu : public AGame {
 public:
  Menu();
  Menu(const Menu &) = default;
  Menu(Menu &&) = delete;
  Menu &operator=(const Menu &) = default;
  Menu &operator=(Menu &&) = delete;
  ~Menu() override = default;

  void init() override;
  void stop() override;
  void restart() override;
  void update(Input input, float deltaTime) override;
};
