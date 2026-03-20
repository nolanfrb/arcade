/*
** EPITECH PROJECT, 2026
** arcade
** File description:
** LibManager
*/

#pragma once

#include <cstddef>
#include <string>
#include <vector>
#include "../../shared/interface/IDisplay.hpp"
#include "../../shared/interface/IGame.hpp"
#include "../dlLoader/dlLoader.hpp"

class LibManager {
 public:
  LibManager();
  ~LibManager();

  LibManager(const LibManager& other) = delete;
  LibManager& operator=(const LibManager& other) = delete;
  LibManager(LibManager&& other) = delete;
  LibManager& operator=(LibManager&& other) = delete;

  void scanLibs(const std::string& path);

  void loadGame(const std::string& path);
  void loadDisplay(const std::string& path);

  void nextGame();
  void nextDisplay();

  [[nodiscard]] IGame* getGame() const { return _game; }
  [[nodiscard]] IDisplay* getDisplay() const { return _display; }

 private:
  dlLoader<IGame> _gameLoader;
  dlLoader<IDisplay> _displayLoader;

  IGame* _game = nullptr;
  IDisplay* _display = nullptr;

  std::vector<std::string> _games;
  std::vector<std::string> _displays;

  std::size_t _gameIndex = 0;
  std::size_t _displayIndex = 0;
};
