/*
** EPITECH PROJECT, 2026
** Arcade
** File description:
** SDL2Renderer
*/

#pragma once

#include <SDL2/SDL_events.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_video.h>
#include <string>
#include <unordered_map>
#include "../../../../shared/Entity.hpp"
#include "../../../../shared/Input.hpp"
#include "../../../../shared/Text.hpp"

class SDL2Renderer {
 public:
  SDL2Renderer() = default;
  ~SDL2Renderer();

  SDL2Renderer(const SDL2Renderer&) = delete;
  SDL2Renderer& operator=(const SDL2Renderer&) = delete;
  SDL2Renderer(SDL2Renderer&&) = delete;
  SDL2Renderer& operator=(SDL2Renderer&&) = delete;

  void init(int width, int height, const std::string& title);
  void close();
  [[nodiscard]] bool isOpen() const;

  static bool pollEvent(SDL_Event& event);
  static Input mapEvent(const SDL_Event& event);
  static char mapTextEvent(const SDL_Event& event);

  void clear();
  void drawEntity(const Entity& entity);
  void drawText(const Text& text);
  void display();

 private:
  SDL_Window* _window = nullptr;
  SDL_Renderer* _renderer = nullptr;
  std::unordered_map<std::string, SDL_Texture*> _textures;
  std::unordered_map<std::string, TTF_Font*> _fonts;

  SDL_Texture* loadTexture(const std::string& path);
  TTF_Font* loadFont(const std::string& path, int size);

  bool drawSprite(const Entity& entity);
  void drawAsciiFallback(const Entity& entity);
  void drawCircle(const Entity& entity);
  void drawTriangle(const Entity& entity);
  void drawRectangle(const Entity& entity);
};
