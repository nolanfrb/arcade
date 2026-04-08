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

class CoreContext : public IGameContext {
 public:
  explicit CoreContext(LibManager& libManager) : _lm(libManager) {}
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
  std::optional<std::string> _pendingGame;
  std::optional<std::string> _pendingDisplay;
};
