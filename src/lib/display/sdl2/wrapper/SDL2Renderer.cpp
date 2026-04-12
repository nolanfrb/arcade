/*
** EPITECH PROJECT, 2026
** Arcade
** File description:
** SDL2Renderer
*/

#include "SDL2Renderer.hpp"
#include <SDL2/SDL.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_keycode.h>
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_surface.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_video.h>
#include <stdexcept>
#include "../../../../shared/Entity.hpp"
#include "../../../../shared/Input.hpp"
#include "../../../../shared/Text.hpp"
#include "../constants/SDL2Constants.hpp"

namespace {
constexpr int MAX_ALPHA = 255;
constexpr float DIAMETER_DIVISOR = 2.F;
constexpr int DIAMETER_MULTIPLIER = 2;
}  // namespace

void SDL2Renderer::init(int width, int height, const std::string& title) {
  SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_AUDIO);
  Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 2, 2048);

  // NOLINTBEGIN(hicpp-signed-bitwise)
  const Uint32 windowFlags = SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE;
  _window =
      SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED,
                       SDL_WINDOWPOS_CENTERED, width, height, windowFlags);
  // NOLINTEND(hicpp-signed-bitwise)

  if (_window == nullptr) {
    throw std::runtime_error("Failed to create SDL window");
  }

  _renderer = SDL_CreateRenderer(
      _window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

  if (_renderer == nullptr) {
    SDL_DestroyWindow(_window);
    _window = nullptr;
    throw std::runtime_error("Failed to create SDL renderer");
  }

  SDL_RenderSetLogicalSize(_renderer, sdl2::LOGICAL_WIDTH,
                           sdl2::LOGICAL_HEIGHT);

  IMG_Init(IMG_INIT_PNG);
  TTF_Init();
}

SDL2Renderer::~SDL2Renderer() { close(); }

void SDL2Renderer::close() {
  for (auto& soundPair : _sounds) {
    if (soundPair.second != nullptr) {
      Mix_FreeChunk(soundPair.second);
    }
  }
  _sounds.clear();

  for (auto& fontPair : _fonts) {
    if (fontPair.second != nullptr) {
      TTF_CloseFont(fontPair.second);
    }
  }
  _fonts.clear();

  for (auto& texturePair : _textures) {
    if (texturePair.second != nullptr) {
      SDL_DestroyTexture(texturePair.second);
    }
  }
  _textures.clear();

  if (_renderer != nullptr) {
    SDL_DestroyRenderer(_renderer);
    _renderer = nullptr;
  }

  if (_window != nullptr) {
    SDL_DestroyWindow(_window);
    _window = nullptr;
  }

  Mix_CloseAudio();
  IMG_Quit();
  TTF_Quit();
  SDL_Quit();
}

bool SDL2Renderer::isOpen() const {
  return _window != nullptr && _renderer != nullptr;
}

bool SDL2Renderer::pollEvent(SDL_Event& event) {
  return SDL_PollEvent(&event) != 0;
}

Input SDL2Renderer::mapEvent(const SDL_Event& event) {
  if (event.type != SDL_KEYDOWN) {
    return Input::NONE;
  }

  switch (event.key.keysym.sym) {
    case SDLK_UP:
      return Input::UP;
    case SDLK_DOWN:
      return Input::DOWN;
    case SDLK_LEFT:
      return Input::LEFT;
    case SDLK_RIGHT:
      return Input::RIGHT;
    case SDLK_SPACE:
      return Input::ACTION;
    case SDLK_ESCAPE:
      return Input::MENU;
    case SDLK_q:
      return Input::EXIT;
    case SDLK_r:
      return Input::RESTART;
    case SDLK_g:
      return Input::NEXTGAME;
    case SDLK_h:
      return Input::NEXTDISPLAY;
    default:
      return Input::NONE;
  }
}

char SDL2Renderer::mapTextEvent(const SDL_Event& event) {
  if (event.type != SDL_TEXTINPUT) {
    return '\0';
  }

  const unsigned char unicode = event.text.text[0];

  if (unicode >= sdl2::PRINTABLE_MIN) {
    return static_cast<char>(unicode);
  }

  return '\0';
}

void SDL2Renderer::clear() {
  SDL_SetRenderDrawColor(_renderer, 0, 0, 0, MAX_ALPHA);
  SDL_RenderClear(_renderer);
}

bool SDL2Renderer::drawSprite(const Entity& entity) {
  SDL_Texture* texture = loadTexture(entity.type.spritePath);

  if (texture == nullptr) {
    return false;
  }

  int textureWidth = 0;
  int textureHeight = 0;
  SDL_QueryTexture(texture, nullptr, nullptr, &textureWidth, &textureHeight);

  const float posX = entity.position.x * sdl2::TILE_SIZE;
  const float posY = entity.position.y * sdl2::TILE_SIZE;
  const float width = entity.type.width * sdl2::TILE_SIZE;
  const float height = entity.type.height * sdl2::TILE_SIZE;

  const SDL_Rect srcRect{.x = 0, .y = 0, .w = textureWidth, .h = textureHeight};
  const SDL_Rect destRect{.x = static_cast<int>(posX),
                          .y = static_cast<int>(posY),
                          .w = static_cast<int>(width),
                          .h = static_cast<int>(height)};

  SDL_RenderCopy(_renderer, texture, &srcRect, &destRect);
  return true;
}

void SDL2Renderer::drawAsciiFallback(const Entity& entity) {
  if (entity.type.asciiChar == '\0') {
    return;
  }

  const Text fallbackText{.content = std::string(1, entity.type.asciiChar),
                          .position = entity.position,
                          .color = entity.type.color,
                          .fontPath = sdl2::DEFAULT_FONT,
                          .fontSize = sdl2::DEFAULT_FONT_SIZE};
  drawText(fallbackText);
}

void SDL2Renderer::drawCircle(const Entity& entity) {
  const float posX = entity.position.x * sdl2::TILE_SIZE;
  const float posY = entity.position.y * sdl2::TILE_SIZE;
  const float width = entity.type.width * sdl2::TILE_SIZE;

  const float radius = width / DIAMETER_DIVISOR;
  const int radiusInt = static_cast<int>(radius);

  for (int widthIdx = 0; widthIdx < radiusInt * DIAMETER_MULTIPLIER;
       widthIdx++) {
    for (int heightIdx = 0; heightIdx < radiusInt * DIAMETER_MULTIPLIER;
         heightIdx++) {
      const int deltaX = radiusInt - widthIdx;
      const int deltaY = radiusInt - heightIdx;

      if (((deltaX * deltaX) + (deltaY * deltaY)) <= (radiusInt * radiusInt)) {
        SDL_RenderDrawPoint(_renderer, static_cast<int>(posX) + widthIdx,
                            static_cast<int>(posY) + heightIdx);
      }
    }
  }
}

void SDL2Renderer::drawTriangle(const Entity& entity) {
  const int posX = static_cast<int>(entity.position.x * sdl2::TILE_SIZE);
  const int posY = static_cast<int>(entity.position.y * sdl2::TILE_SIZE);
  const int width = static_cast<int>(entity.type.width * sdl2::TILE_SIZE);
  const int height = static_cast<int>(entity.type.height * sdl2::TILE_SIZE);

  const SDL_Point point1{.x = posX, .y = posY};
  const SDL_Point point2{.x = posX + width, .y = posY + height};
  const SDL_Point point3{.x = posX - width, .y = posY + height};
  const std::array<SDL_Point, 3> points = {{point1, point2, point3}};

  SDL_RenderDrawLines(_renderer, points.data(),
                      static_cast<int>(points.size()));
  SDL_RenderDrawLine(_renderer, points[2].x, points[2].y, points[0].x,
                     points[0].y);
}

void SDL2Renderer::drawRectangle(const Entity& entity) {
  const SDL_Rect rect{
      .x = static_cast<int>(entity.position.x * sdl2::TILE_SIZE),
      .y = static_cast<int>(entity.position.y * sdl2::TILE_SIZE),
      .w = static_cast<int>(entity.type.width * sdl2::TILE_SIZE),
      .h = static_cast<int>(entity.type.height * sdl2::TILE_SIZE)};

  SDL_RenderFillRect(_renderer, &rect);
}

void SDL2Renderer::drawEntity(const Entity& entity) {
  if (entity.type.type == Shape::SPRITE && !entity.type.spritePath.empty()) {
    if (drawSprite(entity)) {
      return;
    }
    drawAsciiFallback(entity);
    return;
  }

  SDL_SetRenderDrawColor(_renderer, entity.type.color[0], entity.type.color[1],
                         entity.type.color[2], MAX_ALPHA);

  if (entity.type.type == Shape::CIRCLE) {
    drawCircle(entity);
  } else if (entity.type.type == Shape::TRIANGLE) {
    drawTriangle(entity);
  } else {
    drawRectangle(entity);
  }
}

void SDL2Renderer::drawText(const Text& text) {
  if (text.content.empty()) {
    return;
  }
  const std::string& fontPath =
      text.fontPath.empty() ? sdl2::DEFAULT_FONT : text.fontPath;
  TTF_Font* font = loadFont(fontPath, sdl2::DEFAULT_FONT_SIZE);
  if (font == nullptr) {
    return;
  }
  const SDL_Color sdlColor{.r = text.color[0],
                           .g = text.color[1],
                           .b = text.color[2],
                           .a = text.color[3]};
  SDL_Surface* surface =
      TTF_RenderUTF8_Blended(font, text.content.c_str(), sdlColor);
  if (surface == nullptr) {
    return;
  }
  const int surfaceW = surface->w;
  const int surfaceH = surface->h;
  SDL_Texture* texture = SDL_CreateTextureFromSurface(_renderer, surface);
  SDL_FreeSurface(surface);
  if (texture == nullptr) {
    return;
  }
  const SDL_Rect destRect{
      .x = static_cast<int>(text.position.x * sdl2::TILE_SIZE),
      .y = static_cast<int>(text.position.y * sdl2::TILE_SIZE),
      .w = surfaceW,
      .h = surfaceH};

  SDL_RenderCopy(_renderer, texture, nullptr, &destRect);
  SDL_DestroyTexture(texture);
}

void SDL2Renderer::display() { SDL_RenderPresent(_renderer); }

SDL_Texture* SDL2Renderer::loadTexture(const std::string& path) {
  auto iterator = _textures.find(path);

  if (iterator != _textures.end()) {
    return iterator->second;
  }

  SDL_Surface* surface = IMG_Load(path.c_str());

  if (surface == nullptr) {
    _textures[path] = nullptr;
    return nullptr;
  }

  SDL_Texture* texture = SDL_CreateTextureFromSurface(_renderer, surface);
  SDL_FreeSurface(surface);

  if (texture == nullptr) {
    _textures[path] = nullptr;
    return nullptr;
  }

  _textures[path] = texture;
  return texture;
}

TTF_Font* SDL2Renderer::loadFont(const std::string& path, int size) {
  const std::string key = path + "@" + std::to_string(size);
  auto iterator = _fonts.find(key);

  if (iterator != _fonts.end()) {
    return iterator->second;
  }

  TTF_Font* font = TTF_OpenFont(path.c_str(), size);

  if (font == nullptr) {
    throw std::runtime_error("Failed to load font: " + path);
  }

  _fonts[key] = font;
  return font;
}

Mix_Chunk* SDL2Renderer::loadSound(const std::string& path) {
  auto iterator = _sounds.find(path);
  if (iterator != _sounds.end()) {
    return iterator->second;
  }
  Mix_Chunk* chunk = Mix_LoadWAV(path.c_str());
  _sounds[path] = chunk;
  return chunk;
}

void SDL2Renderer::playSound(const Sound& sound) {
  Mix_Chunk* chunk = loadSound(sound.filePath);
  if (chunk == nullptr) {
    return;
  }
  constexpr int MAX_VOLUME = 128;
  Mix_VolumeChunk(chunk, static_cast<int>(sound.volume / 100.F * MAX_VOLUME));
  int channel = Mix_PlayChannel(-1, chunk, sound.loop ? -1 : 0);
  (void)channel;
}
