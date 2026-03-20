/*
** EPITECH PROJECT, 2026
** arcade
** File description:
** core
*/

#include "core.hpp"
#include <filesystem>
#include <iostream>
#include <string>

core::core() {
  _systemCommand.setOnExitRequested([this]() { _running = false; });
  _systemCommand.setOnMenuRequested([this]() { menu(); });
  _systemCommand.setOnRestartRequested([this]() { restart(); });
  _systemCommand.setOnNextGameRequested([this]() { _libManager.nextGame(); });
  _systemCommand.setOnNextDisplayRequested(
      [this]() { _libManager.nextDisplay(); });
}

void core::loadDisplay(std::string const& path) {
  _libManager.loadDisplay(path);
}

void core::loadGame(std::string const& path) { _libManager.loadGame(path); }

void core::menu() {
  if (_libManager.getDisplay() != nullptr) {
    _libManager.getDisplay()->clear();
  }
  if (_libManager.getGame() != nullptr) {
    _libManager.getGame()->stop();
  }
  _libManager.loadGame("./lib/arcade_menu.so");
}

int core::run(std::filesystem::path const& path) {
  _libManager.loadDisplay(path.string());
  _libManager.scanLibs("./lib");

  if (_libManager.getDisplay() == nullptr) {
    std::cerr << "No display library found." << '\n';
    return ERROR;
  }
  _libManager.loadGame("./lib/arcade_menu.so");

  while (_running) {
    IDisplay* currentDisplay = _libManager.getDisplay();
    IGame* currentGame = _libManager.getGame();

    if (currentDisplay != nullptr) {
      Input input = currentDisplay->getEvent();
      _systemCommand.handleSystemEvent(input);

      if (currentGame != nullptr) {
        currentGame->update(input, DEFAULT_DELTA_TIME);
        currentDisplay->clear();
        currentDisplay->drawEntity(currentGame->getEntity());
        currentDisplay->drawText(currentGame->getText());
        currentDisplay->display();
      }
    }
  }

  return 0;
}

void core::restart() {
  if (_libManager.getGame() != nullptr) {
    _libManager.getGame()->stop();
    _libManager.getGame()->init();
  }
}
