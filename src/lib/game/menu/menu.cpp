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
  _selectedIndex = 0;
  _state = MenuState::BROWSING;
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
  if (_state == MenuState::BROWSING) {
    handleBrowsing(input);
  } else {
    handleUsernameInput(input);
  }
  buildScene();
}

void Menu::handleBrowsing(Input input) {
  if (_gameList.empty()) {
    return;
  }
  if (input == Input::UP && _selectedIndex > 0) {
    _selectedIndex--;
  } else if (input == Input::DOWN && _selectedIndex + 1 < _gameList.size()) {
    _selectedIndex++;
  } else if (input == Input::ACTION && _ctx != nullptr) {
    _ctx->loadGame(_gameList[_selectedIndex]);
  }
}

void Menu::handleUsernameInput(Input /*input*/) {}

void Menu::buildScene() {
  Scene scene;
  scene
      .label("Scoreboard: " + std::to_string(0) + " entries",
             {.x = MENU_LEFT_MARGIN, .y = SCOREBOARD_Y})
      .list(_gameList, {.x = MENU_LEFT_MARGIN, .y = GAME_LIST_Y},
            _selectedIndex);

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
