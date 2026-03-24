/*
** EPITECH PROJECT, 2026
** arcade
** File description:
** SystemCommand
*/

#pragma once

#include <array>
#include <functional>
#include <utility>
#include "../../shared/Input.hpp"

class SystemCommand {
 public:
  SystemCommand();
  ~SystemCommand();

  SystemCommand(const SystemCommand& other) = delete;
  SystemCommand& operator=(const SystemCommand& other) = delete;
  SystemCommand(SystemCommand&& other) = delete;
  SystemCommand& operator=(SystemCommand&& other) = delete;

  void handleSystemEvent(Input input);

  void setOnExitRequested(std::function<void()> callback) {
    onExitRequested = std::move(callback);
  }
  void setOnMenuRequested(std::function<void()> callback) {
    onMenuRequested = std::move(callback);
  }
  void setOnNextGameRequested(std::function<void()> callback) {
    onNextGameRequested = std::move(callback);
  }
  void setOnNextDisplayRequested(std::function<void()> callback) {
    onNextDisplayRequested = std::move(callback);
  }

  [[nodiscard]] std::function<void()> getOnExitRequested() const {
    return onExitRequested;
  }
  [[nodiscard]] std::function<void()> getOnMenuRequested() const {
    return onMenuRequested;
  }
  [[nodiscard]] std::function<void()> getOnNextGameRequested() const {
    return onNextGameRequested;
  }
  [[nodiscard]] std::function<void()> getOnNextDisplayRequested() const {
    return onNextDisplayRequested;
  }

 private:
  using EventHandler = void (SystemCommand::*)();

  void handleMenu();
  void handleExit();
  void handleNextGame();
  void handleNextDisplay();

  static constexpr std::array<std::pair<Input, EventHandler>, 4> _eventMap = {{
      {Input::MENU, &SystemCommand::handleMenu},
      {Input::EXIT, &SystemCommand::handleExit},
      {Input::NEXTGAME, &SystemCommand::handleNextGame},
      {Input::NEXTDISPLAY, &SystemCommand::handleNextDisplay},
  }};

  std::function<void()> onExitRequested;
  std::function<void()> onMenuRequested;
  std::function<void()> onNextGameRequested;
  std::function<void()> onNextDisplayRequested;
};
