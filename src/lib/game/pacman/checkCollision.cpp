/*
** EPITECH PROJECT, 2026
** arcade
** File description:
** checkCollision
*/

#include "pacman.hpp"

#define SUPERPACGUM_SCORE 10
#define PACGUM_SCORE 5
#define FOOD_SCORE 1
#define GHOST_SCORE 200

void Pacman::checkBordersCollision() {}

void Pacman::checkSuperPacgumCollision() {
  for (auto it = _superPacgums.begin(); it != _superPacgums.end(); ++it) {
    if (_player.position.x == it->position.x &&
        _player.position.y == it->position.y) {
      _score += SUPERPACGUM_SCORE;
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
      _score += PACGUM_SCORE;
      _pacgums.erase(it);
      break;
    }
  }
}

void Pacman::checkFoodCollision() {
  for (auto it = _foods.begin(); it != _foods.end(); ++it) {
    if (_player.position.x == it->position.x &&
        _player.position.y == it->position.y) {
      _score += FOOD_SCORE;
      _foods.erase(it);
      break;
    }
  }
}

void Pacman::checkGhostCollision() {
  for (size_t i = 0; i < _ghosts.size();) {
    auto& ghost = _ghosts[i];
    if (_player.position.x == ghost.position.x &&
        _player.position.y == ghost.position.y) {
      if (_isSuperPacgumActive) {
        _score += GHOST_SCORE;
        ghost.type = getEntityTypes()[getTypeIndex(type::DEAD_GHOST)];
        _ghostSpawnPositions.push_back(ghost.position);
        _ghostRespawnTimers.push_back(-1);
        _deadGhosts.push_back(ghost);
        _ghosts.erase(_ghosts.begin() + static_cast<int64_t>(i));
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
