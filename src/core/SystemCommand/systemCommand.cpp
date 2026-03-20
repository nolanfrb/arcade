/*
** EPITECH PROJECT, 2026
** arcade
** File description:
** SystemCommand
*/

#include "SystemCommand.hpp"

void SystemCommand::handleMenu() { onMenuRequested(); }
void SystemCommand::handleExit() { onExitRequested(); }
void SystemCommand::handleRestart() { onRestartRequested(); }
void SystemCommand::handleNextGame() { onNextGameRequested(); }
void SystemCommand::handleNextDisplay() { onNextDisplayRequested(); }

void SystemCommand::handleSystemEvent(Input input) {
  for (auto const& [key, val] : _eventMap) {
    if (key == input) {
      (this->*val)();
    }
  }
}
