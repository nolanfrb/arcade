/*
** EPITECH PROJECT, 2026
** arcade
** File description:
** snake food
*/

#include <cstdlib>
#include "snake.hpp"

void Snake::spawnFood() {
  Position newFood;
  bool valid = false;
  const int maxAttempts = PLAY_AREA_WIDTH * PLAY_AREA_HEIGHT;

  for (int attempt = 0; attempt < maxAttempts && !valid; ++attempt) {
    newFood.x = static_cast<float>(PLAY_AREA_X + (rand() % PLAY_AREA_WIDTH));
    newFood.y = static_cast<float>(PLAY_AREA_Y + (rand() % PLAY_AREA_HEIGHT));
    valid = !isPositionOccupied(newFood, true);
  }

  if (!valid) {
    return;
  }

  _foods.push_back({newFood, false, 0.F});
}

void Snake::spawnBonusFood() {
  Position newFood;
  bool valid = false;
  const int maxAttempts = PLAY_AREA_WIDTH * PLAY_AREA_HEIGHT;

  for (int attempt = 0; attempt < maxAttempts && !valid; ++attempt) {
    newFood.x = static_cast<float>(PLAY_AREA_X + (rand() % PLAY_AREA_WIDTH));
    newFood.y = static_cast<float>(PLAY_AREA_Y + (rand() % PLAY_AREA_HEIGHT));
    valid = !isPositionOccupied(newFood, true);
  }

  if (!valid) {
    return;
  }

  _foods.push_back({newFood, true, BONUS_FOOD_DURATION});
}

void Snake::updateBonusFood(float deltaTime) {
  for (auto it = _foods.begin(); it != _foods.end();) {
    if (it->isBonus) {
      it->bonusTimer -= deltaTime;
      if (it->bonusTimer <= 0.F) {
        it = _foods.erase(it);
        if (_foods.empty()) {
          spawnFood();
        }
      } else {
        ++it;
      }
    } else {
      ++it;
    }
  }
}
