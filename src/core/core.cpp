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

Core::Core() {
  _systemCommand.setOnExitRequested([this]() { _running = false; });
  _systemCommand.setOnMenuRequested([this]() { menu(); });
  _systemCommand.setOnNextGameRequested([this]() { _libManager.nextGame(); });
  _systemCommand.setOnNextDisplayRequested(
      [this]() { _libManager.nextDisplay(); });
}

void Core::loadDisplay(std::string const& path) {
  _libManager.loadDisplay(path);
}

void Core::loadGame(std::string const& path) { _libManager.loadGame(path); }

void Core::menu() {
  if (_libManager.getDisplay() != nullptr) {
    _libManager.getDisplay()->clear();
  }
  _libManager.loadGame("./lib/arcade_menu.so");
}

int Core::run(std::filesystem::path const& path) {
  _libManager.loadDisplay(path.string());
  _libManager.scanLibs("./lib");

  if (_libManager.getDisplay() == nullptr) {
    std::cerr << "No display library found." << '\n';
    return ERROR;
  }
  _libManager.loadGame("./lib/arcade_menu.so");
  IDisplay* currentDisplay = _libManager.getDisplay();
  IGame* currentGame = _libManager.getGame();
  while (_running) {
    if (currentDisplay != nullptr) {
      Input input = currentDisplay->getEvent();
      _systemCommand.handleSystemEvent(input);

      currentDisplay = _libManager.getDisplay();
      currentGame = _libManager.getGame();
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
