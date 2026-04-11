/*
** EPITECH PROJECT, 2026
** arcade
** File description:
** CoreContext
*/

#pragma once

#include <optional>
#include <string>
#include <vector>
#include "../shared/interface/IGameContext.hpp"
#include "LibManager/libManager.hpp"
#include "Registry.hpp"

class CoreContext : public IGameContext {
 public:
  CoreContext(LibManager& libManager, Registry& registry)
      : _lm(libManager), _registry(registry) {}
  ~CoreContext() override = default;

  CoreContext(const CoreContext&) = delete;
  CoreContext& operator=(const CoreContext&) = delete;
  CoreContext(CoreContext&&) = delete;
  CoreContext& operator=(CoreContext&&) = delete;

  [[nodiscard]] std::vector<std::string> getGameList() const override {
    return _lm.getGames();
  }

  [[nodiscard]] std::vector<std::string> getDisplayList() const override {
    return _lm.getDisplays();
  }

  void loadGame(const std::string& path) override { _pendingGame = path; }
  void loadDisplay(const std::string& path) override { _pendingDisplay = path; }

  [[nodiscard]] std::string getUsername() const override {
    return _registry.getUsername();
  }
  void setUsername(const std::string& username) override {
    _registry.setUsername(username);
  }
  [[nodiscard]] int getHighScore(const std::string& gameName) const override {
    return _registry.getHighScore(gameName);
  }
  void setHighScore(const std::string& gameName, int score) override {
    _registry.setHighScore(gameName, score);
  }

  void setTextInput(const std::optional<std::string>& text) override {
    _textInput = text;
  }
  [[nodiscard]] std::optional<std::string> getTextInput() const override {
    return _textInput;
  }

  void applyPending() {
    if (_pendingDisplay.has_value()) {
      _lm.loadDisplay(*_pendingDisplay);
      _pendingDisplay.reset();
    }
    if (_pendingGame.has_value()) {
      _lm.loadGame(*_pendingGame);
      _pendingGame.reset();
    }
  }

 private:
  LibManager& _lm;
  Registry& _registry;
  std::optional<std::string> _pendingGame;
  std::optional<std::string> _pendingDisplay;
  std::optional<std::string> _textInput;
};
