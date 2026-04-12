#include "menu.hpp"
#include <filesystem>
#include <string>
#include "../../../shared/Input.hpp"
#include "../../../shared/interface/IGame.hpp"
#include "../../../shared/interface/IGameContext.hpp"
#include "scene.hpp"
namespace {
constexpr float MENU_LEFT_MARGIN = 5.0F;
constexpr float SCOREBOARD_X = 30.0F;
constexpr float SCOREBOARD_Y = 1.0F;
constexpr float GAME_LIST_Y = 3.0F;
constexpr float SCORE_LIST_Y = 5.0F;
constexpr float SCORE_SPACING = 1.5F;
constexpr int SMALL_FONT = 14;

std::string formatLibName(const std::string& path) {
  std::string filename = std::filesystem::path(path).stem().string();
  const std::string prefix = "arcade_";
  if (filename.starts_with(prefix)) {
    filename = filename.substr(prefix.size());
  }
  if (!filename.empty()) {
    filename[0] = static_cast<char>(std::toupper(filename[0]));
  }
  return filename;
}

std::vector<std::string> formatLibNames(const std::vector<std::string>& paths) {
  std::vector<std::string> names;
  names.reserve(paths.size());
  for (const auto& path : paths) {
    names.push_back(formatLibName(path));
  }
  return names;
}
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
  _state = _username.empty() ? MenuState::ENTERING_USERNAME
                             : MenuState::SELECTING_GAME;
}

void Menu::setContext(IGameContext& ctx) {
  _ctx = &ctx;
  _gameList.clear();
  for (const auto& path : ctx.getGameList()) {
    if (std::filesystem::path(path).filename() != "arcade_menu.so") {
      _gameList.push_back(path);
    }
  }
  _displayList = ctx.getDisplayList();
  _username = ctx.getUsername();
  if (!_username.empty()) {
    _state = MenuState::SELECTING_GAME;
  }
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

void Menu::handleUsernameInput(Input input) {
  if (_ctx == nullptr) {
    return;
  }
  auto textInput = _ctx->getTextInput();
  if (textInput.has_value()) {
    _username = *textInput;
  }
  if (input == Input::ACTION && !_username.empty()) {
    _ctx->setUsername(_username);
    _state = MenuState::SELECTING_GAME;
  }
}

void Menu::buildScoreboard(Scene& scene) {
  if (_ctx == nullptr) {
    return;
  }
  scene.label("Player: " + _username,
              {.x = MENU_LEFT_MARGIN, .y = SCOREBOARD_Y}, Colors::YELLOW);
  scene.label("HIGH SCORES", {.x = SCOREBOARD_X, .y = GAME_LIST_Y},
              Colors::GREEN);
  scene.label("-----------", {.x = SCOREBOARD_X, .y = GAME_LIST_Y + 1},
              Colors::GREEN, SMALL_FONT);
  float scoreY = SCORE_LIST_Y;
  for (const auto& path : _gameList) {
    const std::string name = formatLibName(path);
    int score = _ctx->getHighScore(name);
    std::string scoreStr = score > 0 ? std::to_string(score) : "-";
    scene.label(name + ": " + scoreStr, {.x = SCOREBOARD_X, .y = scoreY},
                score > 0 ? Colors::WHITE : Colors::BLUE, SMALL_FONT);
    scoreY += SCORE_SPACING;
  }
}

void Menu::buildScene() {
  Scene scene;
  if (_state == MenuState::ENTERING_USERNAME) {
    scene
        .label("Enter your username:",
               {.x = MENU_LEFT_MARGIN, .y = GAME_LIST_Y}, Colors::GREEN)
        .label("> " + _username + "_",
               {.x = MENU_LEFT_MARGIN, .y = GAME_LIST_Y + 2}, Colors::WHITE)
        .label("Press SPACE to confirm",
               {.x = MENU_LEFT_MARGIN, .y = GAME_LIST_Y + 4}, Colors::YELLOW,
               SMALL_FONT);
  } else if (_state == MenuState::SELECTING_GAME) {
    buildScoreboard(scene);
    scene
        .label("Select a game:", {.x = MENU_LEFT_MARGIN, .y = GAME_LIST_Y},
               Colors::GREEN)
        .list(formatLibNames(_gameList),
              {.x = MENU_LEFT_MARGIN, .y = GAME_LIST_Y + 2},
              _selectedGameIndex);
  } else if (_state == MenuState::SELECTING_DISPLAY) {
    buildScoreboard(scene);
    scene
        .label("Game: " + formatLibName(_gameList[_selectedGameIndex]),
               {.x = MENU_LEFT_MARGIN, .y = GAME_LIST_Y}, Colors::YELLOW)
        .label("Select a display library:",
               {.x = MENU_LEFT_MARGIN, .y = GAME_LIST_Y + 2}, Colors::GREEN)
        .list(formatLibNames(_displayList),
              {.x = MENU_LEFT_MARGIN, .y = GAME_LIST_Y + 4},
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
extern "C" void destroyGame(gsl::owner<IGame*> game) { delete game; }
