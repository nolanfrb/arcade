/*
** EPITECH PROJECT, 2026
** arcade
** File description:
** LibManager
*/

#include "LibManager.hpp"
#include <filesystem>

LibManager::~LibManager() {
  if (_game != nullptr) {
    _game->stop();
    delete _game;
  }
  if (_display != nullptr) {
    _display->stop();
    delete _display;
  }
}

void LibManager::scanLibs(const std::string& path) {
  _games.clear();
  _displays.clear();
  for (auto const& entry : std::filesystem::directory_iterator(path)) {
    if (entry.path().extension() == ".so") {
      if (dlLoader<IGame>::hasSymbol(entry.path(), "createGame")) {
        _games.push_back(entry.path().string());
      } else if (dlLoader<IDisplay>::hasSymbol(entry.path(), "createDisplay")) {
        _displays.push_back(entry.path().string());
      }
    }
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
