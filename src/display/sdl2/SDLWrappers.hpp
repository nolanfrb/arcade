/*
** EPITECH PROJECT, 2026
** Arcade
** File description:
** SDL2 RAII wrappers
*/

#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_error.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_keycode.h>
#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_stdinc.h>
#include <SDL2/SDL_surface.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_video.h>
#include <stdexcept>
#include <string>
#include "../../shared/Input.hpp"

static constexpr Uint8 SDL_ALPHA_OPAQUE_VALUE = 255;

class SDLContext {
 public:
  explicit SDLContext(Uint32 flags = SDL_INIT_VIDEO) {
    if (SDL_Init(flags) != 0) {
      throw std::runtime_error(std::string("SDL_Init: ") + SDL_GetError());
    }
    if (TTF_Init() != 0) {
      SDL_Quit();
      throw std::runtime_error(std::string("TTF_Init: ") + TTF_GetError());
    }
    IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG);
  }

  ~SDLContext() {
    IMG_Quit();
    TTF_Quit();
    SDL_Quit();
  }

  SDLContext(const SDLContext&) = delete;
  SDLContext& operator=(const SDLContext&) = delete;
  SDLContext(SDLContext&&) = delete;
  SDLContext& operator=(SDLContext&&) = delete;
};

class SDLWindow {
 public:
  SDLWindow(const std::string& title, int width, int height,
            Uint32 flags = SDL_WINDOW_SHOWN)
      : _window(SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED,
                                 SDL_WINDOWPOS_CENTERED, width, height,
                                 flags)) {
    if (_window == nullptr) {
      throw std::runtime_error(std::string("SDL_CreateWindow: ") +
                               SDL_GetError());
    }
  }

  ~SDLWindow() {
    if (_window != nullptr) {
      SDL_DestroyWindow(_window);
    }
  }

  SDLWindow(const SDLWindow&) = delete;
  SDLWindow& operator=(const SDLWindow&) = delete;
  SDLWindow(SDLWindow&&) = delete;
  SDLWindow& operator=(SDLWindow&&) = delete;

  [[nodiscard]] SDL_Window* get() const { return _window; }

 private:
  SDL_Window* _window = nullptr;
};

class SDLRenderer {
 public:
  explicit SDLRenderer(SDLWindow& window,
                       Uint32 flags = SDL_RENDERER_ACCELERATED |
                                      SDL_RENDERER_PRESENTVSYNC)
      : _renderer(SDL_CreateRenderer(window.get(), -1, flags)) {
    if (_renderer == nullptr) {
      throw std::runtime_error(std::string("SDL_CreateRenderer: ") +
                               SDL_GetError());
    }
  }

  ~SDLRenderer() {
    if (_renderer != nullptr) {
      SDL_DestroyRenderer(_renderer);
    }
  }

  SDLRenderer(const SDLRenderer&) = delete;
  SDLRenderer& operator=(const SDLRenderer&) = delete;
  SDLRenderer(SDLRenderer&&) = delete;
  SDLRenderer& operator=(SDLRenderer&&) = delete;

  [[nodiscard]] SDL_Renderer* get() const { return _renderer; }

  void setDrawColor(Uint8 red, Uint8 green, Uint8 blue,
                    Uint8 alpha = SDL_ALPHA_OPAQUE_VALUE) {
    SDL_SetRenderDrawColor(_renderer, red, green, blue, alpha);
  }

  void clear() { SDL_RenderClear(_renderer); }

  void fillRect(const SDL_Rect& rect) { SDL_RenderFillRect(_renderer, &rect); }

  void copy(SDL_Texture* tex, const SDL_Rect* src, const SDL_Rect& dst) {
    SDL_RenderCopy(_renderer, tex, src, &dst);
  }

  void present() { SDL_RenderPresent(_renderer); }

 private:
  SDL_Renderer* _renderer = nullptr;
};

class SDLSurface {
 public:
  explicit SDLSurface(SDL_Surface* surface) : _surface(surface) {
    if (_surface == nullptr) {
      throw std::runtime_error(std::string("SDLSurface null: ") +
                               TTF_GetError());
    }
  }

  ~SDLSurface() {
    if (_surface != nullptr) {
      SDL_FreeSurface(_surface);
    }
  }

  SDLSurface(const SDLSurface&) = delete;
  SDLSurface& operator=(const SDLSurface&) = delete;
  SDLSurface(SDLSurface&&) = delete;
  SDLSurface& operator=(SDLSurface&&) = delete;

  [[nodiscard]] SDL_Surface* get() const { return _surface; }

 private:
  SDL_Surface* _surface = nullptr;
};

class SDLTexture {
 public:
  SDLTexture(SDLRenderer& renderer, const std::string& path)
      : _texture(IMG_LoadTexture(renderer.get(), path.c_str())) {
    if (_texture == nullptr) {
      throw std::runtime_error(std::string("IMG_LoadTexture '") + path +
                               "': " + IMG_GetError());
    }
  }

  SDLTexture(SDLRenderer& renderer, SDL_Surface* surface)
      : _texture(SDL_CreateTextureFromSurface(renderer.get(), surface)) {
    if (_texture == nullptr) {
      throw std::runtime_error(std::string("SDL_CreateTextureFromSurface: ") +
                               SDL_GetError());
    }
  }

  ~SDLTexture() {
    if (_texture != nullptr) {
      SDL_DestroyTexture(_texture);
    }
  }

  SDLTexture(const SDLTexture&) = delete;
  SDLTexture& operator=(const SDLTexture&) = delete;

  SDLTexture(SDLTexture&& other) noexcept : _texture(other._texture) {
    other._texture = nullptr;
  }

  SDLTexture& operator=(SDLTexture&& other) noexcept {
    if (this != &other) {
      if (_texture != nullptr) {
        SDL_DestroyTexture(_texture);
      }
      _texture = other._texture;
      other._texture = nullptr;
    }
    return *this;
  }

  [[nodiscard]] SDL_Texture* get() const { return _texture; }

  void setColorMod(Uint8 red, Uint8 green, Uint8 blue) {
    SDL_SetTextureColorMod(_texture, red, green, blue);
  }

  void setAlphaMod(Uint8 alpha) { SDL_SetTextureAlphaMod(_texture, alpha); }

  void querySize(int& width, int& height) const {
    SDL_QueryTexture(_texture, nullptr, nullptr, &width, &height);
  }

 private:
  SDL_Texture* _texture = nullptr;
};

class SDLFont {
 public:
  SDLFont(const std::string& path, int size)
      : _font(TTF_OpenFont(path.c_str(), size)) {
    if (_font == nullptr) {
      throw std::runtime_error(std::string("TTF_OpenFont '") + path +
                               "': " + TTF_GetError());
    }
  }

  ~SDLFont() {
    if (_font != nullptr) {
      TTF_CloseFont(_font);
    }
  }

  SDLFont(const SDLFont&) = delete;
  SDLFont& operator=(const SDLFont&) = delete;
  SDLFont(SDLFont&&) = delete;
  SDLFont& operator=(SDLFont&&) = delete;

  [[nodiscard]] TTF_Font* get() const { return _font; }

  [[nodiscard]] SDLSurface renderBlended(const std::string& text,
                                         SDL_Color color) const {
    return SDLSurface(TTF_RenderText_Blended(_font, text.c_str(), color));
  }

 private:
  TTF_Font* _font = nullptr;
};

class SDLTimer {
 public:
  SDLTimer() : _start(SDL_GetTicks()) {}

  ~SDLTimer() = default;

  SDLTimer(const SDLTimer&) = default;
  SDLTimer& operator=(const SDLTimer&) = default;
  SDLTimer(SDLTimer&&) = default;
  SDLTimer& operator=(SDLTimer&&) = default;

  void reset() { _start = SDL_GetTicks(); }

  [[nodiscard]] Uint32 elapsed() const { return SDL_GetTicks() - _start; }

  static void delay(Uint32 millis) { SDL_Delay(millis); }

 private:
  Uint32 _start;
};

class SDLEventHandler {
 public:
  SDLEventHandler() = default;
  ~SDLEventHandler() = default;

  SDLEventHandler(const SDLEventHandler&) = default;
  SDLEventHandler& operator=(const SDLEventHandler&) = default;
  SDLEventHandler(SDLEventHandler&&) = default;
  SDLEventHandler& operator=(SDLEventHandler&&) = default;

  [[nodiscard]] static Input poll() {
    SDL_Event event;
    while (SDL_PollEvent(&event) != 0) {
      if (event.type == SDL_QUIT) {
        return Input::EXIT;
      }
      if (event.type == SDL_KEYDOWN) {
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
          case SDLK_RETURN:
            return Input::ACTION;
          case SDLK_ESCAPE:
            return Input::EXIT;
          case SDLK_m:
            return Input::MENU;
          case SDLK_r:
            return Input::RESTART;
          default:
            break;
        }
      }
    }
    return Input::NONE;
  }
};
