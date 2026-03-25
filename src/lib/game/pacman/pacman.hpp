/*
** EPITECH PROJECT, 2026
** arcade
** File description:
** pacman
*/

#pragma once

#include <cstdint>
#include <string>
#include "../../../shared/Input.hpp"
#include "../../../shared/abstract/AGame.hpp"

enum class type : std::int8_t {
  WALL,
  PACMAN,
  GHOST,
};

class Pacman : public AGame {
 public:
  Pacman();
  ~Pacman() override;
  Pacman(const Pacman&) = default;
  Pacman& operator=(const Pacman&) = default;
  Pacman(Pacman&&) = default;
  Pacman& operator=(Pacman&&) = default;

  void init() override;
  void stop() override;
  void restart() override;

  void update(Input input, float deltaTime) override;

 private:
  void movePlayer(Input input);
  bool loadMap(const std::string& filePath);
};
