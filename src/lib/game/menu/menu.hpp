/*
** EPITECH PROJECT, 2026
** arcade
** File description:
** menu
*/

#pragma once

#include <cstddef>
#include <string>
#include <vector>
#include "../../../shared/Input.hpp"
#include "../../../shared/abstract/AGame.hpp"
#include "../../../shared/interface/IGameContext.hpp"

enum class MenuState : std::int8_t {
  BROWSING,
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
  void buildEntities();
  void buildTexts();

  void handleBrowsing(Input input);
  void handleUsernameInput(Input input);

  IGameContext* _ctx = nullptr;
  std::vector<std::string> _gameList;
  std::vector<std::string> _displayList;
  std::size_t _selectedIndex = 0;
  std::string _username;
  MenuState _state = MenuState::BROWSING;
};

extern "C" IGame* createGame();
