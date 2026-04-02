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
#include <SDL2/SDL_video.h>
#include <array>
#include <stdexcept>
#include <string>
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
  SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER);

  _window =
      SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED,
                       SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_SHOWN);

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

  IMG_Init(IMG_INIT_PNG);
}

SDL2Renderer::~SDL2Renderer() { close(); }

void SDL2Renderer::close() {
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

  IMG_Quit();
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

void SDL2Renderer::drawEntity(const Entity& entity) {
  if (entity.type.type == Shape::SPRITE && !entity.type.spritePath.empty()) {
    SDL_Texture* texture = loadTexture(entity.type.spritePath);

    if (texture == nullptr) {
      return;
    }

    int textureWidth = 0;
    int textureHeight = 0;
    SDL_QueryTexture(texture, nullptr, nullptr, &textureWidth, &textureHeight);

    const float posX = entity.position.x * sdl2::TILE_SIZE;
    const float posY = entity.position.y * sdl2::TILE_SIZE;
    const float width = entity.type.width * sdl2::TILE_SIZE;
    const float height = entity.type.height * sdl2::TILE_SIZE;

    const SDL_Rect srcRect{
        .x = 0, .y = 0, .w = textureWidth, .h = textureHeight};
    const SDL_Rect destRect{.x = static_cast<int>(posX),
                            .y = static_cast<int>(posY),
                            .w = static_cast<int>(width),
                            .h = static_cast<int>(height)};

    SDL_RenderCopy(_renderer, texture, &srcRect, &destRect);
  } else {
    SDL_SetRenderDrawColor(_renderer, entity.type.color[0],
                           entity.type.color[1], entity.type.color[2],
                           MAX_ALPHA);

    const float posX = entity.position.x * sdl2::TILE_SIZE;
    const float posY = entity.position.y * sdl2::TILE_SIZE;
    const float width = entity.type.width * sdl2::TILE_SIZE;
    const float height = entity.type.height * sdl2::TILE_SIZE;

    if (entity.type.type == Shape::CIRCLE) {
      const float radius = width / DIAMETER_DIVISOR;
      const int radiusInt = static_cast<int>(radius);

      for (int widthIdx = 0; widthIdx < radiusInt * DIAMETER_MULTIPLIER;
           widthIdx++) {
        for (int heightIdx = 0; heightIdx < radiusInt * DIAMETER_MULTIPLIER;
             heightIdx++) {
          const int deltaX = radiusInt - widthIdx;
          const int deltaY = radiusInt - heightIdx;

          if (((deltaX * deltaX) + (deltaY * deltaY)) <=
              (radiusInt * radiusInt)) {
            SDL_RenderDrawPoint(_renderer, static_cast<int>(posX) + widthIdx,
                                static_cast<int>(posY) + heightIdx);
          }
        }
      }
    } else if (entity.type.type == Shape::TRIANGLE) {
      const int posXInt = static_cast<int>(posX);
      const int posYInt = static_cast<int>(posY);
      const int widthInt = static_cast<int>(width);
      const int heightInt = static_cast<int>(height);

      const SDL_Point point1{.x = posXInt, .y = posYInt};
      const SDL_Point point2{.x = posXInt + widthInt, .y = posYInt + heightInt};
      const SDL_Point point3{.x = posXInt - widthInt, .y = posYInt + heightInt};
      const std::array<SDL_Point, 3> points = {{point1, point2, point3}};

      SDL_RenderDrawLines(_renderer, points.data(),
                          static_cast<int>(points.size()));
      SDL_RenderDrawLine(_renderer, points[2].x, points[2].y, points[0].x,
                         points[0].y);
    } else {
      const SDL_Rect rect{.x = static_cast<int>(posX),
                          .y = static_cast<int>(posY),
                          .w = static_cast<int>(width),
                          .h = static_cast<int>(height)};

      SDL_RenderFillRect(_renderer, &rect);
    }
  }
}

void SDL2Renderer::drawText(const Text& /*text*/) {}

void SDL2Renderer::display() { SDL_RenderPresent(_renderer); }

SDL_Texture* SDL2Renderer::loadTexture(const std::string& path) {
  auto iterator = _textures.find(path);

  if (iterator != _textures.end()) {
    return iterator->second;
  }

  SDL_Surface* surface = IMG_Load(path.c_str());

  if (surface == nullptr) {
    throw std::runtime_error("Failed to load image: " + path);
  }

  SDL_Texture* texture = SDL_CreateTextureFromSurface(_renderer, surface);
  SDL_FreeSurface(surface);

  if (texture == nullptr) {
    throw std::runtime_error("Failed to create texture from: " + path);
  }

  _textures[path] = texture;
  return texture;
}
