/*
** EPITECH PROJECT, 2026
** arcade
** File description:
** core
*/

#pragma once

#include <cstddef>
#include <string>
#include "LibManager/LibManager.hpp"
#include "SystemCommand/SystemCommand.hpp"

#define ERROR 84

constexpr float DEFAULT_DELTA_TIME = 0.016F;

class core {
 public:
  core();
  ~core();

  core(const core& other) = delete;
  core& operator=(const core& other) = delete;
  core(core&& other) = delete;
  core& operator=(core&& other) = delete;

  int run(std::filesystem::path const& path);
  void menu();
  void restart();
  void loadGame(std::string const& path);
  void loadDisplay(std::string const& path);

 private:
  bool _running = true;

  SystemCommand _systemCommand;
  LibManager _libManager;
};
