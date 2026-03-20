/*
** EPITECH PROJECT, 2026
** arcade
** File description:
** LibManager
*/

#include "libManager.hpp"
#include <filesystem>
#include <iostream>

LibManager::~LibManager() {
  if (_game != nullptr) {
    _game->stop();
    delete _game;
    _game = nullptr;
  }
  if (_display != nullptr) {
    _display->stop();
    delete _display;
    _display = nullptr;
  }
}

void LibManager::checkAndAddLib(const std::string& filepath) {
  if (dlLoader<IGame>::hasSymbol(filepath, "createGame")) {
    _games.push_back(filepath);
  } else if (dlLoader<IDisplay>::hasSymbol(filepath, "createDisplay")) {
    _displays.push_back(filepath);
  }
}

void LibManager::scanLibs(const std::string& path) {
  _games.clear();
  _displays.clear();
  if (!std::filesystem::exists(path)) {
    std::cerr << "Warning: Directory or file '" << path << "' not found."
              << '\n';
    return;
  }
  if (std::filesystem::is_directory(path)) {
    for (auto const& entry : std::filesystem::directory_iterator(path)) {
      if (entry.is_regular_file() && entry.path().extension() == ".so") {
        checkAndAddLib(entry.path().string());
      }
    }
  } else if (std::filesystem::is_regular_file(path) &&
             std::filesystem::path(path).extension() == ".so") {
    checkAndAddLib(path);
  }
}

void LibManager::loadGame(const std::string& path) {
  if (_game != nullptr) {
    _game->stop();
    delete _game;
    _game = nullptr;
  }
  _gameLoader.loadLib(path);
  _game = _gameLoader.getInstance("createGame");
  if (_game != nullptr) {
    _game->init();
  }
}

void LibManager::loadDisplay(const std::string& path) {
  if (_display != nullptr) {
    _display->stop();
    delete _display;
    _display = nullptr;
  }
  _displayLoader.loadLib(path);
  _display = _displayLoader.getInstance("createDisplay");
  if (_display != nullptr) {
    _display->init();
  }
}

void LibManager::nextGame() {
  if (_games.empty()) {
    return;
  }
  _gameIndex = (_gameIndex + 1) % _games.size();
  loadGame(_games[_gameIndex]);
}

void LibManager::nextDisplay() {
  if (_displays.empty()) {
    return;
  }
  _displayIndex = (_displayIndex + 1) % _displays.size();
  loadDisplay(_displays[_displayIndex]);
}
