/*
** EPITECH PROJECT, 2026
** arcade
** File description:
** SystemCommand
*/

#include "SystemCommand.hpp"

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
void SystemCommand::handleRestart() {
  if (onRestartRequested) {
    onRestartRequested();
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
    }
  }
}
