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
  for (auto& superPacgum : _superPacgums) {
    if (_player.position.x == superPacgum.position.x &&
        _player.position.y == superPacgum.position.y) {
      _score += SUPERPACGUM_SCORE;
      _isSuperPacgumActive = true;
      _superPacgumTimer = 0;
      std::erase_if(_superPacgums, [&superPacgum](const Entity& entity) {
        return entity.position.x == superPacgum.position.x &&
               entity.position.y == superPacgum.position.y;
      });
      break;
    }
  }
}

void Pacman::checkPacgumCollision() {
  for (auto& pacgum : _pacgums) {
    if (_player.position.x == pacgum.position.x &&
        _player.position.y == pacgum.position.y) {
      _score += PACGUM_SCORE;
      std::erase_if(_pacgums, [&pacgum](const Entity& entity) {
        return entity.position.x == pacgum.position.x &&
               entity.position.y == pacgum.position.y;
      });
      break;
    }
  }
}

void Pacman::checkFoodCollision() {
  for (auto& food : _foods) {
    if (_player.position.x == food.position.x &&
        _player.position.y == food.position.y) {
      _score += FOOD_SCORE;
      std::erase_if(_foods, [&food](const Entity& entity) {
        return entity.position.x == food.position.x &&
               entity.position.y == food.position.y;
      });
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
        _ghostDirections.erase(_ghostDirections.begin() +
                               static_cast<int64_t>(i));
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
