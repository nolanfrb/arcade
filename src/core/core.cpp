/*
** EPITECH PROJECT, 2026
** arcade
** File description:
** core
*/

#include "core.hpp"

core::core() {
  _systemCommand.getOnExitRequested() = [this]() { _running = false; };
  _systemCommand.getOnMenuRequested() = [this]() { menu(); };
  _systemCommand.getOnRestartRequested() = [this]() { restart(); };
  _systemCommand.getOnNextGameRequested() = [this]() {
    _libManager.nextGame();
  };
  _systemCommand.getOnNextDisplayRequested() = [this]() {
    _libManager.nextDisplay();
  };
}

core::~core() {
  if (_game != nullptr) {
    _game->stop();
    delete _game;
  }
  if (_display != nullptr) {
    _display->stop();
    delete _display;
  }
}

void core::loadDisplay(std::string const& path) {
  displayLoader.loadLib(path);
  _display = displayLoader.getInstance("createDisplay");
  _display->init();
}

void core::loadGame(std::string const& path) {
  gameLoader.loadLib(path);
  _game = gameLoader.getInstance("createGame");
  _game->init();
}

void core::menu() {
  _display->clear();
  _game->stop();
  _libManager.loadGame("Menu");
  _libManager.loadDisplay("Ncurses");
}

void core::run() {
  while (_running) {
    _systemCommand.handleSystemEvent(_display->getEvent());
    if (_display != nullptr && _game != nullptr) {
      _display->clear();
      _display->drawEntity(_game->getEntity());
      _display->drawText(_game->getText());
      _display->display();
    }
  }
  if (_display != nullptr) {
    _display->stop();
  }
  if (_game != nullptr) {
    _game->stop();
  }
}

void core::restart() {
  _game->stop();
  _game->init();
}
