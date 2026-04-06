/*
** EPITECH PROJECT, 2026
** arcade
** File description:
** pacman
*/

#pragma once

#include <array>
#include <cstdint>
#include <string>
#include <vector>
#include "../../../shared/Entity.hpp"
#include "../../../shared/Input.hpp"
#include "../../../shared/Position.hpp"
#include "../../../shared/abstract/AGame.hpp"

enum class type : std::int8_t {
  WALL,
  GHOST_DOOR,
  EMPTY,
  PACMAN,
  RED_GHOST,
  PINK_GHOST,
  BLUE_GHOST,
  ORANGE_GHOST,
  SUPERPACGUM,
  PACGUM,
  FOOD,
  CHASED_GHOST,
  DEAD_GHOST,
};

enum class itemScore : std::uint8_t {
  SUPERPACGUM_SCORE = 10,
  PACGUM_SCORE = 1,
  FOOD_SCORE = 20,
  GHOST_SCORE = 200
};

constexpr float GHOST_SPEED_DEAD = 0.2F;
constexpr float GHOST_SPEED_NORMAL = 0.3F;
constexpr float GHOST_SPEED_FRIGHTENED = 0.5F;
constexpr float GHOST_RESPAWN_DELAY = 5.F;
constexpr float GHOST_SPAWN_DELAY = 10.F;
constexpr float LEVEL_UP_SPEED_INCREASE = 0.25F;
constexpr float PLAYER_MOVE_DELAY = 0.15F;

constexpr std::array<std::uint8_t, 4> WHITE = {255, 255, 255, 255};
constexpr std::array<std::uint8_t, 4> BLUE = {0, 0, 255, 255};
constexpr std::array<std::uint8_t, 4> RED = {255, 0, 0, 255};
constexpr std::array<std::uint8_t, 4> ORANGE = {255, 165, 0, 255};
constexpr std::array<std::uint8_t, 4> PINK = {255, 192, 203, 255};

class Pacman : public AGame {
 public:
  Pacman() = default;
  ~Pacman() override = default;
  Pacman(const Pacman&) = default;
  Pacman& operator=(const Pacman&) = default;
  Pacman(Pacman&&) = default;
  Pacman& operator=(Pacman&&) = default;

  void init() override;
  void stop() override;
  void restart() override;

  void update(Input input, float deltaTime) override;

 private:
  void updateEntities(std::vector<Entity>& entities,
                      const std::vector<EntityType>& entityTypes,
                      float deltaTime);

  void movePlayer(Input input);
  void moveGhosts(float deltaTime, Input playerInput);
  void moveAliveGhosts(Position target, bool canPassDoor, Input playerInput);
  void moveDeadGhosts(Entity& ghost);

  bool loadMap(const std::string& filePath);

  bool checkMap();
  void checkCollision();

  void checkSuperPacgumCollision();
  void checkPacgumCollision();
  void checkFoodCollision();
  void checkGhostCollision();
  void checkBordersCollision();
  void checkVictory();

  void checkSuperPacgumTimer(float deltaTime);
  void updateAnimation(float deltaTime);

  void createEntities();
  void createEntitiesType();

  type getTile(int xCoordinate, int yCoordinate);
  static int getTypeIndex(type tile);

  void displayGameOver();

  int _score = 0;

  bool _isSuperPacgumActive = false;
  float _superPacgumTimer = 0;
  float _gameTimer = 0;

  float _aliveGhostMovementTimer = 0;
  float _deadGhostMovementTimer = 0;
  float _playerMovementTimer = 0;
  float _animationTimer = 0;

  Input _lastPlayerInput = Input::NONE;

  std::uint8_t _pacmanAnimationFrame = 0;
  std::uint8_t _ghostAnimationFrame = 0;
  std::uint8_t _scaredGhostAnimationFrame = 0;
  std::uint8_t _deadGhostAnimationFrame = 0;

  Entity _player;
  std::vector<Entity> _ghosts;
  std::vector<Input> _ghostDirections;
  std::vector<Entity> _chassedGhosts;
  std::vector<Entity> _deadGhosts;
  std::vector<Position> _ghostSpawnPositions;
  std::vector<float> _ghostRespawnTimers;
  std::vector<Entity> _superPacgums;
  std::vector<Entity> _pacgums;
  std::vector<Entity> _foods;
  std::vector<std::vector<type>> _map;

  float _ghostSpeedMultiplier = 1;
};
