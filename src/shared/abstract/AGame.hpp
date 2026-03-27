/*
** EPITECH PROJECT, 2026
** arcade
** File description:
** AGame
*/

#pragma once

#include <string>
#include <vector>
#include "../Entity.hpp"
#include "../Input.hpp"
#include "../Text.hpp"
#include "../interface/IGame.hpp"

class AGame : public IGame {
 public:
  AGame() = default;

  ~AGame() override = default;

  AGame(const AGame&) = default;
  AGame& operator=(const AGame&) = default;
  AGame(AGame&&) = default;
  AGame& operator=(AGame&&) = default;

  void init() override = 0;
  void stop() override = 0;
  void restart() override = 0;

  void update(Input input, float deltaTime) override = 0;

  std::vector<Entity> getEntity() override { return _entities; }
  std::vector<Text> getText() override { return _texts; }
  int getScore() override { return _score; }
  std::string getName() override { return _name; }

  bool isGameOver() override { return _isGameOver; }

 protected:
  void addEntity(const Entity& entity) { _entities.push_back(entity); }
  void addText(const Text& text) { _texts.push_back(text); }

  void setScore(int score) { _score = score; }
  void setIsGameOver(bool isGameOver) { _isGameOver = isGameOver; }
  void setName(const std::string& name) { _name = name; }
  void setEntities(const std::vector<Entity>& entities) {
    _entities = entities;
  }
  void setEntityTypes(const std::vector<EntityType>& entityTypes) {
    _entityTypes = entityTypes;
  }
  [[nodiscard]] const std::vector<EntityType>& getEntityTypes() const {
    return _entityTypes;
  }

  void clearEntities() { _entities.clear(); }
  void clearTexts() { _texts.clear(); }

 private:
  std::vector<Entity> _entities;
  std::vector<EntityType> _entityTypes;
  std::vector<Text> _texts;
  int _score = 0;
  bool _isGameOver = false;
  std::string _name;
};
