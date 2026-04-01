#include "menu.hpp"
#include <filesystem>
#include <string>
#include "../../../shared/Input.hpp"
#include "../../../shared/interface/IGame.hpp"
#include "../../../shared/interface/IGameContext.hpp"
#include "scene.hpp"
namespace {
constexpr float MENU_LEFT_MARGIN = 5.0F;
constexpr float SCOREBOARD_Y = 1.0F;
constexpr float GAME_LIST_Y = 3.0F;
}  // namespace

namespace gsl {
template <typename T>
using owner = T;
}  // namespace gsl

void Menu::init() { setName("menu"); }
void Menu::stop() {}
void Menu::restart() {
  _selectedGameIndex = 0;
  _selectedDisplayIndex = 0;
  _state = MenuState::SELECTING_GAME;
}

void Menu::setContext(IGameContext* ctx) {
  _ctx = ctx;
  if (ctx == nullptr) {
    return;
  }
  _gameList.clear();
  for (const auto& path : ctx->getGameList()) {
    if (std::filesystem::path(path).filename() != "arcade_menu.so") {
      _gameList.push_back(path);
    }
  }
  _displayList = ctx->getDisplayList();
}

void Menu::update(Input input, float /*deltaTime*/) {
  switch (_state) {
    case MenuState::SELECTING_GAME:
      handleGameSelection(input);
      break;
    case MenuState::SELECTING_DISPLAY:
      handleDisplaySelection(input);
      break;
    case MenuState::ENTERING_USERNAME:
      handleUsernameInput(input);
      break;
  }
  buildScene();
}

void Menu::handleGameSelection(Input input) {
  if (_gameList.empty()) {
    return;
  }
  if (input == Input::UP && _selectedGameIndex > 0) {
    _selectedGameIndex--;
  } else if (input == Input::DOWN &&
             _selectedGameIndex + 1 < _gameList.size()) {
    _selectedGameIndex++;
  } else if (input == Input::ACTION) {
    _selectedDisplayIndex = 0;
    _state = MenuState::SELECTING_DISPLAY;
  }
}

void Menu::handleDisplaySelection(Input input) {
  if (_displayList.empty()) {
    return;
  }
  if (input == Input::UP && _selectedDisplayIndex > 0) {
    _selectedDisplayIndex--;
  } else if (input == Input::DOWN &&
             _selectedDisplayIndex + 1 < _displayList.size()) {
    _selectedDisplayIndex++;
  } else if (input == Input::ACTION && _ctx != nullptr) {
    _ctx->loadDisplay(_displayList[_selectedDisplayIndex]);
    _ctx->loadGame(_gameList[_selectedGameIndex]);
  } else if (input == Input::MENU) {
    _state = MenuState::SELECTING_GAME;
  }
}

void Menu::handleUsernameInput(Input /*input*/) {}

void Menu::buildScene() {
  Scene scene;
  if (_state == MenuState::SELECTING_GAME) {
    scene
        .label("Select a game:", {.x = MENU_LEFT_MARGIN, .y = GAME_LIST_Y},
               Colors::GREEN)
        .list(_gameList, {.x = MENU_LEFT_MARGIN, .y = GAME_LIST_Y + 2},
              _selectedGameIndex);
  } else if (_state == MenuState::SELECTING_DISPLAY) {
    scene
        .label("Game: " + _gameList[_selectedGameIndex],
               {.x = MENU_LEFT_MARGIN, .y = GAME_LIST_Y}, Colors::YELLOW)
        .label("Select a display library:",
               {.x = MENU_LEFT_MARGIN, .y = GAME_LIST_Y + 2}, Colors::GREEN)
        .list(_displayList, {.x = MENU_LEFT_MARGIN, .y = GAME_LIST_Y + 4},
              _selectedDisplayIndex);
  }

  clearTexts();
  clearEntities();
  for (const auto& text : scene.texts()) {
    addText(text);
  }
  for (const auto& entity : scene.entities()) {
    addEntity(entity);
  }
}

extern "C" gsl::owner<IGame*> createGame() { return new Menu(); }
