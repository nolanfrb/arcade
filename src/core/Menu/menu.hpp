/*
** EPITECH PROJECT, 2026
** arcade
** File description:
** menu
*/

#pragma once

#include "../../shared/abstract/AGame.hpp"

class menu : public AGame {
 public:
  menu();
  ~menu() override;

  menu(const menu& other) = delete;
  menu& operator=(const menu& other) = delete;
  menu(menu&& other) = delete;
  menu& operator=(menu&& other) = delete;

  void init() override;
  void stop() override;
  void restart() override;
  void update(Input input, float deltaTime) override;

 private:
};
