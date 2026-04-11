/*
** EPITECH PROJECT, 2026
** Arcade
** File description:
** sfmlRenderer
*/

#pragma once

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>
#include "../../../../shared/Entity.hpp"
#include "../../../../shared/Input.hpp"
#include "../../../../shared/Sound.hpp"
#include "../../../../shared/Text.hpp"

class SfmlRenderer {
 public:
  SfmlRenderer() = default;
  ~SfmlRenderer() = default;

  SfmlRenderer(const SfmlRenderer&) = delete;
  SfmlRenderer& operator=(const SfmlRenderer&) = delete;
  SfmlRenderer(SfmlRenderer&&) = delete;
  SfmlRenderer& operator=(SfmlRenderer&&) = delete;

  void init(unsigned int width, unsigned int height, const std::string& title);
  void close();
  bool isOpen() const;

  bool pollEvent(sf::Event& event);
  static Input mapEvent(const sf::Event& event);
  static char mapTextEvent(const sf::Event& event);

  void clear();
  void drawEntity(const Entity& entity);
  void drawText(const Text& text);
  void playSound(const Sound& sound);
  void display();

 private:
  sf::RenderWindow _window;
  std::unordered_map<std::string, sf::Texture> _textures;
  std::unordered_map<std::string, sf::Font> _fonts;

  sf::Texture& loadTexture(const std::string& path);
  sf::Font& loadFont(const std::string& path);
  sf::SoundBuffer& loadSoundBuffer(const std::string& path);

  void drawShape(const Entity& entity);
  void drawSprite(const Entity& entity);

  std::unordered_map<std::string, sf::SoundBuffer> _soundBuffers;
  std::vector<std::unique_ptr<sf::Sound>> _activeSounds;
};
