/*
** EPITECH PROJECT, 2026
** arcade
** File description:
** core
*/

#pragma once

#include <cstddef>
#include <string>
#include <vector>
#include "../shared/interface/IDisplay.hpp"
#include "../shared/interface/IGame.hpp"
#include "LibManager/LibManager.hpp"
#include "SystemCommand/SystemCommand.hpp"
#include "dlLoader/dlLoader.hpp"

#define ERROR 84

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
  dlLoader<IDisplay> displayLoader;
  dlLoader<IGame> gameLoader;

  std::vector<std::string> _games;
  std::vector<std::string> _displays;

  std::size_t _gameIndex = 0;
  std::size_t _displayIndex = 0;

  IDisplay* _display = nullptr;
  IGame* _game = nullptr;
  bool _running = true;

  SystemCommand _systemCommand;
  LibManager _libManager;
};
