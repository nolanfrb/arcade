/*
** EPITECH PROJECT, 2026
** Arcade
** File description:
** SDL2Display
*/

#pragma once

#include <map>
#include <memory>
#include <optional>
#include <string>
#include <vector>
#include "../../shared/Entity.hpp"
#include "../../shared/Input.hpp"
#include "../../shared/Text.hpp"
#include "../../shared/abstract/ADisplay.hpp"
#include "../../shared/interface/IDisplay.hpp"
#include "SDLWrappers.hpp"

static constexpr int CELL_SIZE = 32;
static constexpr int WINDOW_W = 1280;
static constexpr int WINDOW_H = 720;
static constexpr int TARGET_FPS = 60;
static constexpr int FRAME_DELAY = 1000 / TARGET_FPS;

class SDL2Display : public ADisplay {
 public:
  SDL2Display();
  ~SDL2Display() override = default;

  SDL2Display(const SDL2Display&) = delete;
  SDL2Display& operator=(const SDL2Display&) = delete;
  SDL2Display(SDL2Display&&) = delete;
  SDL2Display& operator=(SDL2Display&&) = delete;

  void init() override;
  void stop() override;

  Input getEvent() override;
  std::optional<std::string> getTextInput() override;

  void clear() override;
  void drawEntity(const std::vector<Entity>& entities) override;
  void drawText(const std::vector<Text>& texts) override;
  void display() override;

 private:
  std::unique_ptr<SDLContext> _context;
  std::unique_ptr<SDLWindow> _window;
  std::unique_ptr<SDLRenderer> _renderer;
  std::unique_ptr<SDLFont> _font;
  [[maybe_unused]] SDLEventHandler _events;
  SDLTimer _timer;

  std::map<std::string, SDLTexture> _textureCache;

  Entity _playerEntity{};
  bool _hasPlayer = false;

  SDLTexture& loadTexture(const std::string& path);
  void renderEntity(const Entity& entity);
  void renderPlayerOnTop();
};

extern "C" IDisplay* createDisplay();
