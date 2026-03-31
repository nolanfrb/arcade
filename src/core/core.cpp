/*
** EPITECH PROJECT, 2026
** arcade
** File description:
** core
*/

#include "core.hpp"
#include <chrono>
#include <filesystem>
#include <iostream>
#include <string>
#include "../shared/Input.hpp"
#include "../shared/interface/IDisplay.hpp"
#include "../shared/interface/IGame.hpp"
#include "LibManager/libManager.hpp"
#include "SystemCommand/systemCommand.hpp"

namespace {
float getDeltaTime(std::chrono::steady_clock::time_point& lastFrameTime) {
  const auto currentFrameTime = std::chrono::steady_clock::now();
  const float deltaTime =
      std::chrono::duration<float>(currentFrameTime - lastFrameTime).count();
  lastFrameTime = currentFrameTime;

  if (deltaTime <= 0) {
    return DEFAULT_DELTA_TIME;
  }
  if (deltaTime > MAX_DELTA_TIME) {
    return MAX_DELTA_TIME;
  }
  return deltaTime;
}
}  // namespace

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
  _libManager.setContext(&_ctx);

  if (_libManager.getDisplay() == nullptr) {
    std::cerr << "No display library found." << '\n';
    return ERROR;
  }
  _libManager.loadGame("./lib/arcade_menu.so");
  IDisplay* currentDisplay = nullptr;
  IGame* currentGame = nullptr;
  auto lastFrameTime = std::chrono::steady_clock::now();
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
      const float deltaTime = getDeltaTime(lastFrameTime);
      currentGame->update(input, deltaTime);
      _ctx.applyPending();
      currentDisplay = _libManager.getDisplay();
      currentGame = _libManager.getGame();
      if (currentGame == nullptr || currentDisplay == nullptr) {
        continue;
      }

      currentDisplay->clear();
      currentDisplay->drawEntity(currentGame->getEntity());
      currentDisplay->drawText(currentGame->getText());
      currentDisplay->display();
    }
  }
  return 0;
}
