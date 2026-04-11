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
#include "../Sound.hpp"
#include "../Text.hpp"
#include "../interface/IGame.hpp"
#include "../interface/IGameContext.hpp"

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
  void setContext(IGameContext& /*ctx*/) override {}

  std::vector<Entity> getEntity() override { return _entities; }
  std::vector<Text> getText() override { return _texts; }
  std::vector<Sound> getSounds() override {
    std::vector<Sound> sounds = _sounds;
    _sounds.clear();
    return sounds;
  }
  int getScore() override { return _score; }
  std::string getName() override { return _name; }

  bool isGameOver() override { return _isGameOver; }

 protected:
  void addEntity(const Entity& entity) { _entities.push_back(entity); }
  void addText(const Text& text) { _texts.push_back(text); }
  void addSound(const Sound& sound) { _sounds.push_back(sound); }

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
  void clearSounds() { _sounds.clear(); }

 private:
  std::vector<Entity> _entities;
  std::vector<EntityType> _entityTypes;
  std::vector<Text> _texts;
  mutable std::vector<Sound> _sounds;
  int _score = 0;
  bool _isGameOver = false;
  std::string _name;
};
