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
#include "../shared/Input.hpp"
#include "../shared/interface/IDisplay.hpp"
#include "../shared/interface/IGame.hpp"
#include "LibManager/libManager.hpp"
#include "SystemCommand/systemCommand.hpp"

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
  IDisplay* currentDisplay = nullptr;
  IGame* currentGame = nullptr;
  while (_running) {
    currentDisplay = _libManager.getDisplay();
    if (currentDisplay == nullptr) {
      std::cerr << "Display library became unavailable." << '\n';
      return ERROR;
    }
    const Input input = currentDisplay->getEvent();
    _systemCommand.handleSystemEvent(input);
    currentDisplay = _libManager.getDisplay();
    currentGame = _libManager.getGame();
    if (currentDisplay == nullptr) {
      std::cerr << "Display library became unavailable during execution."
                << '\n';
      return ERROR;
    }
    if (currentGame != nullptr) {
      currentGame->update(input, DEFAULT_DELTA_TIME);
      currentDisplay->clear();
      currentDisplay->drawEntity(currentGame->getEntity());
      currentDisplay->drawText(currentGame->getText());
      currentDisplay->display();
    }
  }
  return 0;
}
