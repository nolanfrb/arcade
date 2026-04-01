/*
** EPITECH PROJECT, 2026
** arcade
** File description:
** core
*/

#include "core.hpp"
#include <array>
#include <chrono>
#include <cstdint>
#include <filesystem>
#include <iostream>
#include <string>
#include <vector>
#include "../shared/Entity.hpp"
#include "../shared/Input.hpp"
#include "../shared/Text.hpp"
#include "../shared/interface/IDisplay.hpp"
#include "../shared/interface/IGame.hpp"
#include "LibManager/libManager.hpp"
#include "SystemCommand/systemCommand.hpp"

namespace {
constexpr std::array<std::uint8_t, 4> SCORE_TEXT_COLOR = {255, 255, 255, 255};
constexpr int SCORE_TEXT_FONT_SIZE = 16;
constexpr float SCORE_TEXT_X = 1.0F;
constexpr float SCORE_TEXT_Y = 0.0F;

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

void createScore(IGame* currentGame, std::vector<Text>& texts) {
  if (currentGame->getName() != "menu") {
    Text scoreText;
    scoreText.content = "Score: " + std::to_string(currentGame->getScore());
    scoreText.position = {.x = SCORE_TEXT_X, .y = SCORE_TEXT_Y};
    scoreText.color = SCORE_TEXT_COLOR;
    scoreText.fontSize = SCORE_TEXT_FONT_SIZE;
    texts.push_back(scoreText);
  }
}
}  // namespace

Core::Core() {
  _systemCommand.setOnExitRequested([this]() { _running = false; });
  _systemCommand.setOnMenuRequested([this]() { menu(); });
  _systemCommand.setOnNextGameRequested([this]() { _libManager.nextGame(); });
  _systemCommand.setOnNextDisplayRequested(
      [this]() { _libManager.nextDisplay(); });
  _systemCommand.setOnRestartRequested([this]() {
    IGame* game = _libManager.getGame();
    if (game != nullptr) {
      game->restart();
    }
  });
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

int Core::initCore(std::filesystem::path const& path) {
  _libManager.loadDisplay(path.string());
  _libManager.scanLibs("./lib");
  _libManager.setContext(&_ctx);

  if (_libManager.getDisplay() == nullptr) {
    std::cerr << "No display library found." << '\n';
    return ERROR;
  }
  _libManager.loadGame("./lib/arcade_menu.so");
  return 0;
}

void Core::updateGame(IGame* currentGame, IDisplay* currentDisplay, Input input,
                      std::chrono::steady_clock::time_point& lastFrameTime) {
  const float deltaTime = getDeltaTime(lastFrameTime);
  currentGame->update(input, deltaTime);
  _ctx.applyPending();
  currentDisplay = _libManager.getDisplay();
  currentGame = _libManager.getGame();
  if (currentGame == nullptr || currentDisplay == nullptr) {
    return;
  }
  currentDisplay->clear();
  const std::vector<Entity> entities = currentGame->getEntity();
  std::vector<Text> texts = currentGame->getText();
  createScore(currentGame, texts);
  currentDisplay->drawEntity(entities);
  currentDisplay->drawText(texts);
  currentDisplay->display();
}

int Core::run(std::filesystem::path const& path) {
  if (initCore(path) != 0) {
    return ERROR;
  }
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
      updateGame(currentGame, currentDisplay, input, lastFrameTime);
    }
  }
  return 0;
}
