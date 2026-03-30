#include "menu.hpp"
#include <filesystem>
#include "../Scene.hpp"

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
  if (input == Input::UP && _selectedIndex > 0) {
    _selectedIndex--;
  } else if (input == Input::DOWN && _selectedIndex < _gameList.size() - 1) {
    _selectedIndex++;
  } else if (input == Input::ACTION && _ctx != nullptr && !_gameList.empty()) {
    _ctx->loadGame(_gameList[_selectedIndex]);
  }
}

void Menu::handleUsernameInput(Input /*input*/) {}

void Menu::buildScene() {
  Scene scene;
  scene
      .label("Scoreboard: " + std::to_string(0) + " entries",
             {.x = 5, .y = 1.0F})
      .list(_gameList, {.x = 5, .y = 3.0F}, _selectedIndex);

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
