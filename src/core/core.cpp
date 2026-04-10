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
#include "../shared/Sound.hpp"
#include "../shared/Text.hpp"
#include "../shared/interface/IDisplay.hpp"
#include "../shared/interface/IGame.hpp"
#include "LibManager/libManager.hpp"
#include "SystemCommand/systemCommand.hpp"

namespace {
constexpr std::array<std::uint8_t, 4> CONFIRM_TEXT_COLOR = {255, 255, 255, 255};
constexpr std::array<std::uint8_t, 4> CONFIRM_HINT_COLOR = {200, 200, 200, 255};
constexpr int CONFIRM_FONT_SIZE = 16;
constexpr int CONFIRM_HINT_FONT_SIZE = 14;
constexpr float SCREEN_TILES_X = 40.F;
constexpr float SCREEN_TILES_Y = 30.F;
constexpr float SCREEN_CENTER_Y = SCREEN_TILES_Y / 2.F;
constexpr float TILE_PX = 20.F;
constexpr float SCREEN_WIDTH_PX = SCREEN_TILES_X * TILE_PX;

float getCenteredTextPosX(const std::string& text, int fontSize) {
  const float textWidthPx =
      static_cast<float>(text.size()) * static_cast<float>(fontSize);
  return (SCREEN_WIDTH_PX - textWidthPx) / (2.F * TILE_PX);
}

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
  _systemCommand.setOnMenuRequested([this]() {
    IGame* game = _libManager.getGame();
    if (isInMenu() || (game != nullptr && game->isGameOver())) {
      menu();
    } else {
      _confirmingQuit = !_confirmingQuit;
    }
  });
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

bool Core::isInMenu() {
  IGame* game = _libManager.getGame();
  return game != nullptr && game->getName() == "menu";
}

void Core::loadDisplay(std::string const& path) {
  _libManager.loadDisplay(path);
}

void Core::loadGame(std::string const& path) { _libManager.loadGame(path); }

void Core::menu() {
  _confirmingQuit = false;
  IGame* game = _libManager.getGame();
  if (game != nullptr && game->getName() != "menu") {
    _registry.setHighScore(game->getName(), game->getScore());
  }
  if (_libManager.getDisplay() != nullptr) {
    _libManager.getDisplay()->clear();
  }
  _libManager.loadGame("./lib/arcade_menu.so");
}

int Core::initCore(std::filesystem::path const& path) {
  _libManager.loadDisplay(path.string());
  _libManager.scanLibs("./lib");
  _libManager.setContext(_ctx);

  if (_libManager.getDisplay() == nullptr) {
    std::cerr << "No display library found." << '\n';
    return ERROR;
  }
  _libManager.loadGame("./lib/arcade_menu.so");
  return 0;
}

bool Core::handleConfirmQuit(Input input) {
  if (input == Input::ACTION) {
    _confirmingQuit = false;
    menu();
    return true;
  }
  return false;
}

void Core::appendConfirmTexts(std::vector<Text>& texts) {
  const std::string confirmMsg = "Voulez-vous vraiment quitter ?";
  const std::string hintMsg = "Espace pour confirmer / Echap pour annuler";

  Text confirmText;
  confirmText.content = confirmMsg;
  confirmText.fontSize = CONFIRM_FONT_SIZE;
  confirmText.position =
      Position{.x = getCenteredTextPosX(confirmMsg, CONFIRM_FONT_SIZE),
               .y = SCREEN_CENTER_Y - 1.F};
  confirmText.color = CONFIRM_TEXT_COLOR;
  confirmText.fontPath = "";
  texts.push_back(confirmText);

  Text hintText;
  hintText.content = hintMsg;
  hintText.fontSize = CONFIRM_HINT_FONT_SIZE;
  hintText.position =
      Position{.x = getCenteredTextPosX(hintMsg, CONFIRM_HINT_FONT_SIZE),
               .y = SCREEN_CENTER_Y + 1.F};
  hintText.color = CONFIRM_HINT_COLOR;
  hintText.fontPath = "";
  texts.push_back(hintText);
}

void Core::updateGame(IGame* currentGame, IDisplay* currentDisplay, Input input,
                      std::chrono::steady_clock::time_point& lastFrameTime) {
  const float deltaTime = getDeltaTime(lastFrameTime);
  _ctx.setTextInput(currentDisplay->getTextInput());
  if (_confirmingQuit) {
    if (handleConfirmQuit(input)) {
      return;
    }
    currentGame->update(Input::NONE, 0.F);
  } else {
    currentGame->update(input, deltaTime);
  }
  _ctx.applyPending();
  currentDisplay = _libManager.getDisplay();
  currentGame = _libManager.getGame();
  if (currentGame == nullptr || currentDisplay == nullptr) {
    return;
  }
  currentDisplay->clear();
  const std::vector<Entity> entities = currentGame->getEntity();
  std::vector<Text> texts = currentGame->getText();
  if (_confirmingQuit) {
    appendConfirmTexts(texts);
  }
  const std::vector<Sound> sounds = currentGame->getSounds();
  currentDisplay->drawEntity(entities);
  currentDisplay->drawText(texts);
  if (!_confirmingQuit) {
    currentDisplay->playSound(sounds);
  }
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
