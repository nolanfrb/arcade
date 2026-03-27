#include "menu.hpp"
#include <array>
#include <filesystem>

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
  buildTexts();
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

void Menu::buildEntities() {}

void Menu::buildTexts() {
  clearTexts();
  for (std::size_t i = 0; i < _gameList.size(); i++) {
    Text t;
    t.content = (i == _selectedIndex ? "> " : "  ") + _gameList[i];
    t.position = {.x = 5, .y = 3.0F + (static_cast<float>(i) * 2)};
    t.color = (i == _selectedIndex)
                  ? std::array<uint8_t, 4>{255, 255, 0, 255}
                  : std::array<uint8_t, 4>{255, 255, 255, 255};
    addText(t);
  }
}

extern "C" gsl::owner<IGame*> createGame() { return new Menu(); }
