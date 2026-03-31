/*
** EPITECH PROJECT, 2026
** arcade
** File description:
** menu
*/

#pragma once

#include <cstddef>
#include <cstdint>
#include <string>
#include <vector>
#include "../../../shared/Input.hpp"
#include "../../../shared/abstract/AGame.hpp"
#include "../../../shared/interface/IGame.hpp"
#include "../../../shared/interface/IGameContext.hpp"

enum class MenuState : std::int8_t {
  SELECTING_GAME,
  SELECTING_DISPLAY,
  ENTERING_USERNAME,
};

class Menu : public AGame {
 public:
  Menu() = default;
  ~Menu() override = default;
  Menu(const Menu&) = default;
  Menu& operator=(const Menu&) = default;
  Menu(Menu&&) = default;
  Menu& operator=(Menu&&) = default;

  void init() override;
  void stop() override;
  void restart() override;
  void update(Input input, float deltaTime) override;

  void setContext(IGameContext* ctx) override;

 private:
  void buildScene();

  void handleGameSelection(Input input);
  void handleDisplaySelection(Input input);
  void handleUsernameInput(Input input);
  IGameContext* _ctx = nullptr;
  std::vector<std::string> _gameList;
  std::vector<std::string> _displayList;
  std::size_t _selectedGameIndex = 0;
  std::size_t _selectedDisplayIndex = 0;
  std::string _username;
  MenuState _state = MenuState::SELECTING_GAME;
};

extern "C" IGame* createGame();
