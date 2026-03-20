/*
** EPITECH PROJECT, 2026
** arcade
** File description:
** SystemCommand
*/

#pragma once

#include <array>
#include <functional>
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

  [[nodiscard]] std::function<void()> getOnExitRequested() const {
    return onExitRequested;
  }
  [[nodiscard]] std::function<void()> getOnMenuRequested() const {
    return onMenuRequested;
  }
  [[nodiscard]] std::function<void()> getOnRestartRequested() const {
    return onRestartRequested;
  }
  [[nodiscard]] std::function<void()> getOnNextGameRequested() const {
    return onNextGameRequested;
  }
  [[nodiscard]] std::function<void()> getOnNextDisplayRequested() const {
    return onNextDisplayRequested;
  }

 protected:
 private:
  using EventHandler = void (SystemCommand::*)();

  void handleMenu();
  void handleExit();
  void handleRestart();
  void handleNextGame();
  void handleNextDisplay();
  void handleGameInput();

  static constexpr std::array<std::pair<Input, EventHandler>, 5> _eventMap = {{
      {Input::MENU, &SystemCommand::handleMenu},
      {Input::EXIT, &SystemCommand::handleExit},
      {Input::RESTART, &SystemCommand::handleRestart},
      {Input::NEXTGAME, &SystemCommand::handleNextGame},
      {Input::NEXTDISPLAY, &SystemCommand::handleNextDisplay},
  }};

  std::function<void()> onExitRequested;
  std::function<void()> onMenuRequested;
  std::function<void()> onRestartRequested;
  std::function<void()> onNextGameRequested;
  std::function<void()> onNextDisplayRequested;
};
