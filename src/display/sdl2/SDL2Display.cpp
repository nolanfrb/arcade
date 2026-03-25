/*
** EPITECH PROJECT, 2026
** Arcade
** File description:
** SDL2Display
*/

#include "SDL2Display.hpp"
#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_stdinc.h>
#include <memory>
#include <string>
#include <string_view>
#include <vector>
#include "../../shared/Entity.hpp"
#include "../../shared/Input.hpp"
#include "../../shared/Text.hpp"
#include "../../shared/interface/IDisplay.hpp"
#include "SDLWrappers.hpp"

static constexpr std::string_view FONT_PATH_FALLBACK =
    "/usr/share/fonts/TTF/DejaVuSans.ttf";
static constexpr int FONT_SIZE_DEFAULT = 16;
static constexpr Uint8 COLOR_MAX = 255;
static constexpr Uint8 BG_R = 10;
static constexpr Uint8 BG_G = 10;
static constexpr Uint8 BG_B = 20;
static constexpr SDL_Color WHITE = {
    .r = COLOR_MAX, .g = COLOR_MAX, .b = COLOR_MAX, .a = COLOR_MAX};

SDL2Display::SDL2Display() { setName("sdl2"); }

void SDL2Display::init() {
  _context = std::make_unique<SDLContext>();
  _window = std::make_unique<SDLWindow>("Arcade - SDL2", WINDOW_W, WINDOW_H);
  _renderer = std::make_unique<SDLRenderer>(*_window);

  try {
    _font = std::make_unique<SDLFont>(std::string(FONT_PATH_FALLBACK),
                                      FONT_SIZE_DEFAULT);
  } catch (...) {
    _font = nullptr;
  }

  _timer.reset();
}

void SDL2Display::stop() {
  _textureCache.clear();
  _font.reset();
  _renderer.reset();
  _window.reset();
  _context.reset();
}

Input SDL2Display::getEvent() { return SDLEventHandler::poll(); }

SDLTexture& SDL2Display::loadTexture(const std::string& path) {
  auto iter = _textureCache.find(path);
  if (iter != _textureCache.end()) {
    return iter->second;
  }
  auto [inserted, success] =
      _textureCache.emplace(path, SDLTexture(*_renderer, path));
  (void)success;
  return inserted->second;
}

void SDL2Display::renderEntity(const Entity& entity) {
  const SDL_Rect dst{.x = static_cast<int>(entity.position.x) * CELL_SIZE,
                     .y = static_cast<int>(entity.position.y) * CELL_SIZE,
                     .w = CELL_SIZE,
                     .h = CELL_SIZE};

  if (!entity.texturePath.empty()) {
    try {
      SDLTexture& tex = loadTexture(entity.texturePath);
      tex.setColorMod(entity.color[0], entity.color[1], entity.color[2]);
      tex.setAlphaMod(entity.color[3]);
      _renderer->copy(tex.get(), nullptr, dst);
      return;
    } catch (...) {
      _renderer->setDrawColor(entity.color[0], entity.color[1], entity.color[2],
                              entity.color[3]);
      _renderer->fillRect(dst);
    }
  } else {
    _renderer->setDrawColor(entity.color[0], entity.color[1], entity.color[2],
                            entity.color[3]);
    _renderer->fillRect(dst);
  }

  if (_font != nullptr && entity.asciiChar != '\0') {
    try {
      const SDLSurface surf =
          _font->renderBlended(std::string(1, entity.asciiChar), WHITE);
      const SDLTexture charTex(*_renderer, surf.get());
      int texWidth = 0;
      int texHeight = 0;
      charTex.querySize(texWidth, texHeight);
      const SDL_Rect charDst{.x = dst.x + ((CELL_SIZE - texWidth) / 2),
                             .y = dst.y + ((CELL_SIZE - texHeight) / 2),
                             .w = texWidth,
                             .h = texHeight};
      _renderer->copy(charTex.get(), nullptr, charDst);
    } catch (...) {
      return;
    }
  }
}

void SDL2Display::clear() {
  _renderer->setDrawColor(BG_R, BG_G, BG_B, COLOR_MAX);
  _renderer->clear();
  _hasPlayer = false;
}

void SDL2Display::drawEntity(const std::vector<Entity>& entities) {
  for (const auto& entity : entities) {
    if (entity.isPlayer) {
      _playerEntity = entity;
      _hasPlayer = true;
    } else {
      renderEntity(entity);
    }
  }
}

void SDL2Display::renderPlayerOnTop() {
  if (_hasPlayer) {
    renderEntity(_playerEntity);
  }
}

void SDL2Display::drawText(const std::vector<Text>& texts) {
  if (_font == nullptr) {
    return;
  }
  for (const auto& text : texts) {
    try {
      const SDL_Color color{.r = text.color[0],
                            .g = text.color[1],
                            .b = text.color[2],
                            .a = text.color[3]};
      const SDLSurface surf = _font->renderBlended(text.text, color);
      const SDLTexture tex(*_renderer, surf.get());
      int texWidth = 0;
      int texHeight = 0;
      tex.querySize(texWidth, texHeight);
      const SDL_Rect dst{.x = static_cast<int>(text.position.x),
                         .y = static_cast<int>(text.position.y),
                         .w = texWidth,
                         .h = texHeight};
      _renderer->copy(tex.get(), nullptr, dst);
    } catch (...) {
      continue;
    }
  }
}

void SDL2Display::display() {
  renderPlayerOnTop();
  _renderer->present();
  const Uint32 elapsed = _timer.elapsed();
  if (elapsed < static_cast<Uint32>(FRAME_DELAY)) {
    SDLTimer::delay(FRAME_DELAY - elapsed);
  }
  _timer.reset();
}

extern "C" IDisplay* createDisplay() { return new SDL2Display(); }
