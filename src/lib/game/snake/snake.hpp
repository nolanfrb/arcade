/*
** EPITECH PROJECT, 2026
** arcade
** File description:
** snake
*/

#pragma once

#include <array>
#include <cstdint>
#include <ctime>
#include <deque>
#include <string>
#include <vector>
#include "../../../shared/Entity.hpp"
#include "../../../shared/Input.hpp"
#include "../../../shared/Position.hpp"
#include "../../../shared/Text.hpp"
#include "../../../shared/abstract/AGame.hpp"

constexpr int SCREEN_TILES_X = 40;
constexpr int SCREEN_TILES_Y = 30;
constexpr int MAP_WIDTH = 38;
constexpr int MAP_HEIGHT = 28;
constexpr int PLAY_AREA_X = 1;
constexpr int PLAY_AREA_Y = 1;
constexpr int PLAY_AREA_WIDTH = 36;
constexpr int PLAY_AREA_HEIGHT = 26;
constexpr float MAP_OFFSET_X =
    static_cast<float>(SCREEN_TILES_X - MAP_WIDTH) / 2.F;
constexpr float MAP_OFFSET_Y = 2.F;
constexpr float HUD_Y = 0.5F;
constexpr bool CYCLICAL = false;
constexpr float INITIAL_SPEED = 0.15F;
constexpr float MAX_SPEED = 0.05F;
constexpr float SPEED_INCREMENT = 0.002F;
constexpr float BOOST_SPEED = 0.02F;
constexpr float BOOST_COOLDOWN = 1.F;
constexpr float BONUS_FOOD_SPAWN_CHANCE = 0.08F;
constexpr float BONUS_FOOD_DURATION = 8.F;
constexpr int BONUS_FOOD_SCORE = 50;
constexpr int NORMAL_FOOD_SCORE = 10;
constexpr float TILE_SIZE = 1.F;
constexpr std::array<std::uint8_t, 4> GREEN = {0, 255, 0, 255};
constexpr std::array<std::uint8_t, 4> DARK_GREEN = {0, 200, 0, 255};
constexpr std::array<std::uint8_t, 4> RED = {255, 0, 0, 255};
constexpr std::array<std::uint8_t, 4> GOLD = {255, 215, 0, 255};
constexpr std::array<std::uint8_t, 4> WHITE = {255, 255, 255, 255};
constexpr std::array<std::uint8_t, 4> BLACK = {0, 0, 0, 255};

enum class EntityTypeSnake : std::int8_t {
  SNAKE_HEAD_UP,
  SNAKE_HEAD_DOWN,
  SNAKE_HEAD_LEFT,
  SNAKE_HEAD_RIGHT,
  SNAKE_BODY,
  SNAKE_TAIL,
  FOOD,
  BONUS_FOOD,
  BORDER
};

struct SnakeSegment {
  Position position;
  EntityTypeSnake type;
};

struct FoodItem {
  Position position;
  bool isBonus;
  float bonusTimer;
};

class Snake : public AGame {
 public:
  Snake() = default;
  ~Snake() override = default;
  Snake(const Snake&) = default;
  Snake& operator=(const Snake&) = default;
  Snake(Snake&&) = default;
  Snake& operator=(Snake&&) = default;

  void init() override;
  void stop() override;
  void restart() override;

  void update(Input input, float deltaTime) override;

 private:
  void createBorders();
  void moveSnake(float deltaTime);
  void handleInput(Input input);
  void spawnFood();
  void spawnBonusFood();
  bool canMoveThisFrame(float deltaTime);
  bool prepareNextMove(Position& nextHead);
  bool eatFoodAt(const Position& nextHead);
  void handleFoodSpawnAfterEating();
  void trimSnakeTail();
  void updateEntities();
  void addSnakeEntities();
  void addFoodEntities();
  void updateGameOverText();
  void displayHUD();
  void updateBonusFood(float deltaTime);
  static Position toScreenPos(const Position& pos);
  bool isPositionOccupied(const Position& pos, bool includeBody = true);
  Position getNextHeadPosition();
  EntityTypeSnake getHeadTypeForDirection();

  std::deque<SnakeSegment> _snake;
  std::vector<FoodItem> _foods;
  Input _currentDirection = Input::RIGHT;
  Input _nextDirection = Input::RIGHT;
  float _moveTimer = 0.F;
  float _currentSpeed = INITIAL_SPEED;
  float _boostCooldown = 0.F;
  bool _isBoosting = false;
  int _foodEaten = 0;
};
