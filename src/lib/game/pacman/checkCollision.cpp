/*
** EPITECH PROJECT, 2026
** arcade
** File description:
** checkCollision
*/

#include <cstddef>
#include "pacman.hpp"

void Pacman::checkBordersCollision() {}

void Pacman::checkSuperPacgumCollision() {
  for (auto it = _superPacgums.begin(); it != _superPacgums.end(); ++it) {
    if (_player.position.x == it->position.x &&
        _player.position.y == it->position.y) {
      _score += static_cast<int>(itemScore::SUPERPACGUM_SCORE);
      _isSuperPacgumActive = true;
      _superPacgumTimer = 0;

      _superPacgums.erase(it);
      break;
    }
  }
}

void Pacman::checkPacgumCollision() {
  for (auto it = _pacgums.begin(); it != _pacgums.end(); ++it) {
    if (_player.position.x == it->position.x &&
        _player.position.y == it->position.y) {
      _score += static_cast<int>(itemScore::PACGUM_SCORE);
      _pacgums.erase(it);
      break;
    }
  }
}

void Pacman::checkFoodCollision() {
  for (auto it = _foods.begin(); it != _foods.end(); ++it) {
    if (_player.position.x == it->position.x &&
        _player.position.y == it->position.y) {
      _score += static_cast<int>(itemScore::FOOD_SCORE);
      _foods.erase(it);
      break;
    }
  }
}

void Pacman::checkGhostCollision() {
  for (std::size_t i = 0; i < _ghosts.size();) {
    auto& ghost = _ghosts[i];
    if (_player.position.x == ghost.position.x &&
        _player.position.y == ghost.position.y) {
      if (_isSuperPacgumActive) {
        _score += static_cast<int>(itemScore::GHOST_SCORE);
        _ghostRespawnTimers.push_back(0.F);
        _deadGhosts.push_back(ghost);
        _ghosts.erase(_ghosts.begin() + static_cast<int>(i));
        _ghostDirections.erase(_ghostDirections.begin() + static_cast<int>(i));
        continue;
      }
      setIsGameOver(true);
    }
    ++i;
  }
}

void Pacman::checkCollision() {
  checkGhostCollision();
  checkSuperPacgumCollision();
  checkPacgumCollision();
  checkFoodCollision();
}
