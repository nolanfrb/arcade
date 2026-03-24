/*
** EPITECH PROJECT, 2026
** arcade
** File description:
** SystemCommand
*/

#include "systemCommand.hpp"
#include "../../shared/Input.hpp"

SystemCommand::SystemCommand() = default;
SystemCommand::~SystemCommand() = default;

void SystemCommand::handleMenu() {
  if (onMenuRequested) {
    onMenuRequested();
  }
}
void SystemCommand::handleExit() {
  if (onExitRequested) {
    onExitRequested();
  }
}
void SystemCommand::handleNextGame() {
  if (onNextGameRequested) {
    onNextGameRequested();
  }
}
void SystemCommand::handleNextDisplay() {
  if (onNextDisplayRequested) {
    onNextDisplayRequested();
  }
}

void SystemCommand::handleSystemEvent(Input input) {
  for (auto const& [key, val] : _eventMap) {
    if (key == input) {
      (this->*val)();
      break;
    }
  }
}
