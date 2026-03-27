/*
** EPITECH PROJECT, 2026
** arcade
** File description:
** pacman
*/

#pragma once

#include <cstdint>
#include <string>
#include "../../../shared/Input.hpp"
#include "../../../shared/abstract/AGame.hpp"

enum class type : std::int8_t {
  WALL,
  GHOST_DOOR,
  EMPTY,
  PACMAN,
  GHOST,
  SUPERPACGUM,
  PACGUM,
  FOOD,
  CHASED_GHOST,
  DEAD_GHOST,
};

enum class itemScore : std::uint8_t {
  SUPERPACGUM_SCORE = 10,
  PACGUM_SCORE = 5,
  FOOD_SCORE = 1,
  GHOST_SCORE = 200
};

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
                      const std::vector<EntityType>& entityTypes);

  void movePlayer(Input input);
  void moveGhosts(float deltaTime);
  void moveDeadGhosts(float deltaTime);

  bool loadMap(const std::string& filePath);

  bool checkMap();
  void checkCollision();

  void checkSuperPacgumCollision();
  void checkPacgumCollision();
  void checkFoodCollision();
  void checkGhostCollision();
  void checkSuperPacgumTimer(float deltaTime);

  void checkBordersCollision();

  void createEntities();
  void createEntitiesType();
  type getTile(int xCoordinate, int yCoordinate);

  static int getTypeIndex(type tile);

  Entity _player;
  int _score = 0;
  bool _isSuperPacgumActive = false;
  float _superPacgumTimer = 0;
  std::vector<Entity> _ghosts;
  std::vector<Input> _ghostDirections;
  float _ghostMovementTimer = 0;
  std::vector<Entity> _chassedGhosts;
  std::vector<Entity> _deadGhosts;
  std::vector<Position> _ghostSpawnPositions;
  std::vector<float> _ghostRespawnTimers;
  std::vector<Entity> _superPacgums;
  std::vector<Entity> _pacgums;
  std::vector<Entity> _foods;

  std::vector<std::vector<type>> _map;
};
