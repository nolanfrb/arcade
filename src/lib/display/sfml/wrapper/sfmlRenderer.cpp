/*
** EPITECH PROJECT, 2026
** Arcade
** File description:
** sfmlRenderer
*/

#include "sfmlRenderer.hpp"
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <stdexcept>
#include <string>
#include "../../../../shared/Input.hpp"
#include "../constants/sfmlConstants.hpp"

// NOLINTNEXTLINE(bugprone-easily-swappable-parameters)
void SfmlRenderer::init(unsigned int width, unsigned int height,
                        const std::string& title) {
  // NOLINTBEGIN(hicpp-signed-bitwise)
  const sf::Uint32 windowStyle =
      sf::Style::Titlebar | sf::Style::Close | sf::Style::Resize;
  // NOLINTEND(hicpp-signed-bitwise)
  _window.create(sf::VideoMode(width, height), title, windowStyle);
  _window.setFramerateLimit(sfml::FRAMERATE_LIMIT);
  _view.reset(sf::FloatRect(0.F, 0.F, static_cast<float>(sfml::LOGICAL_WIDTH),
                            static_cast<float>(sfml::LOGICAL_HEIGHT)));
  handleResize(width, height);
}

// NOLINTNEXTLINE(readability-convert-member-functions-to-static)
void SfmlRenderer::handleResize(unsigned int width, unsigned int height) {
  if (width == 0 || height == 0) {
    return;
  }
  const float windowAspect =
      static_cast<float>(width) / static_cast<float>(height);
  const float targetAspect = static_cast<float>(sfml::LOGICAL_WIDTH) /
                             static_cast<float>(sfml::LOGICAL_HEIGHT);
  sf::FloatRect viewport(0.F, 0.F, 1.F, 1.F);
  if (windowAspect > targetAspect) {
    const float viewportWidth = targetAspect / windowAspect;
    viewport =
        sf::FloatRect((1.F - viewportWidth) / 2.F, 0.F, viewportWidth, 1.F);
  } else if (windowAspect < targetAspect) {
    const float viewportHeight = windowAspect / targetAspect;
    viewport =
        sf::FloatRect(0.F, (1.F - viewportHeight) / 2.F, 1.F, viewportHeight);
  }
  _view.setViewport(viewport);
  _window.setView(_view);
}

void SfmlRenderer::close() {
  _window.close();
  _textures.clear();
  _fonts.clear();
}

bool SfmlRenderer::isOpen() const { return _window.isOpen(); }

bool SfmlRenderer::pollEvent(sf::Event& event) {
  return _window.pollEvent(event);
}

Input SfmlRenderer::mapEvent(const sf::Event& event) {
  if (event.type != sf::Event::KeyPressed) {
    return Input::NONE;
  }
  switch (event.key.code) {
    case sf::Keyboard::Up:
      return Input::UP;
    case sf::Keyboard::Down:
      return Input::DOWN;
    case sf::Keyboard::Left:
      return Input::LEFT;
    case sf::Keyboard::Right:
      return Input::RIGHT;
    case sf::Keyboard::Space:
      return Input::ACTION;
    case sf::Keyboard::Escape:
      return Input::MENU;
    case sf::Keyboard::Q:
      return Input::EXIT;
    case sf::Keyboard::R:
      return Input::RESTART;
    case sf::Keyboard::G:
      return Input::NEXTGAME;
    case sf::Keyboard::H:
      return Input::NEXTDISPLAY;
    default:
      return Input::NONE;
  }
}

char SfmlRenderer::mapTextEvent(const sf::Event& event) {
  if (event.type != sf::Event::TextEntered) {
    return '\0';
  }
  const sf::Uint32 unicode = event.text.unicode;
  if (unicode >= sfml::PRINTABLE_MIN && unicode <= sfml::PRINTABLE_MAX) {
    return static_cast<char>(unicode);
  }
  if (unicode == sfml::BACKSPACE_CODE) {
    return '\b';
  }
  return '\0';
}

void SfmlRenderer::clear() { _window.clear(sf::Color::Black); }

void SfmlRenderer::drawEntity(const Entity& entity) {
  if (entity.type.type == Shape::SPRITE && !entity.type.spritePath.empty()) {
    drawSprite(entity);
  } else {
    drawShape(entity);
  }
}

void SfmlRenderer::drawShape(const Entity& entity) {
  const sf::Color color(entity.type.color[0], entity.type.color[1],
                        entity.type.color[2], entity.type.color[3]);
  const float posx = entity.position.x * sfml::TILE_SIZE;
  const float posy = entity.position.y * sfml::TILE_SIZE;
  const float width = entity.type.width * sfml::TILE_SIZE;
  const float height = entity.type.height * sfml::TILE_SIZE;

  switch (entity.type.type) {
    case Shape::CIRCLE: {
      const float radius = width / 2.F;
      sf::CircleShape circle(radius);
      circle.setPosition(posx, posy);
      circle.setFillColor(color);
      _window.draw(circle);
      break;
    }
    case Shape::TRIANGLE: {
      sf::CircleShape triangle(width / 2.F, 3);
      triangle.setPosition(posx, posy);
      triangle.setFillColor(color);
      _window.draw(triangle);
      break;
    }
    default: {
      sf::RectangleShape rect(sf::Vector2f(width, height));
      rect.setPosition(posx, posy);
      rect.setFillColor(color);
      _window.draw(rect);
      break;
    }
  }
}

void SfmlRenderer::drawSprite(const Entity& entity) {
  try {
    const sf::Texture& texture = loadTexture(entity.type.spritePath);
    sf::Sprite sprite(texture);
    const auto texSize = texture.getSize();
    sprite.setPosition(entity.position.x * sfml::TILE_SIZE,
                       entity.position.y * sfml::TILE_SIZE);
    const float scaleX =
        (entity.type.width * sfml::TILE_SIZE) / static_cast<float>(texSize.x);
    const float scaleY =
        (entity.type.height * sfml::TILE_SIZE) / static_cast<float>(texSize.y);
    sprite.setScale(scaleX, scaleY);
    _window.draw(sprite);
  } catch (const std::runtime_error&) {
    drawShape(entity);
  }
}

void SfmlRenderer::drawText(const Text& text) {
  const std::string& fontPath =
      text.fontPath.empty() ? std::string(sfml::DEFAULT_FONT) : text.fontPath;
  const sf::Font& font = loadFont(fontPath);
  sf::Text sfText;
  sfText.setFont(font);
  sfText.setString(text.content);
  sfText.setCharacterSize(text.fontSize);
  sfText.setFillColor(
      sf::Color(text.color[0], text.color[1], text.color[2], text.color[3]));
  sfText.setPosition(text.position.x * sfml::TILE_SIZE,
                     text.position.y * sfml::TILE_SIZE);
  _window.draw(sfText);
}

void SfmlRenderer::display() { _window.display(); }

// We have desactivated linter for "_" because its convention for unused vars.
sf::Texture& SfmlRenderer::loadTexture(const std::string& path) {
  auto iterator = _textures.find(path);
  if (iterator != _textures.end()) {
    return iterator->second;
  }
  sf::Texture texture;
  if (!texture.loadFromFile(path)) {
    throw std::runtime_error("Failed to load texture: " + path);
  }
  auto [inserted, success] = _textures.emplace(path, std::move(texture));
  (void)success;
  return inserted->second;
}

sf::SoundBuffer& SfmlRenderer::loadSoundBuffer(const std::string& path) {
  auto iterator = _soundBuffers.find(path);
  if (iterator != _soundBuffers.end()) {
    return iterator->second;
  }
  auto& buffer = _soundBuffers[path];
  if (!buffer.loadFromFile(path)) {
    _soundBuffers.erase(path);
    throw std::runtime_error("Failed to load sound: " + path);
  }
  return buffer;
}

void SfmlRenderer::playSound(const Sound& sound) {
  // Clean up finished sounds
  _activeSounds.erase(std::remove_if(_activeSounds.begin(), _activeSounds.end(),
                                     [](const std::unique_ptr<sf::Sound>& s) {
                                       return s->getStatus() ==
                                              sf::Sound::Stopped;
                                     }),
                      _activeSounds.end());
  try {
    sf::SoundBuffer& buffer = loadSoundBuffer(sound.filePath);
    auto sfSound = std::make_unique<sf::Sound>(buffer);
    sfSound->setVolume(sound.volume);
    sfSound->setLoop(sound.loop);
    sfSound->play();
    _activeSounds.push_back(std::move(sfSound));
  } catch (const std::runtime_error&) {
  }
}

sf::Font& SfmlRenderer::loadFont(const std::string& path) {
  auto iterator = _fonts.find(path);
  if (iterator != _fonts.end()) {
    return iterator->second;
  }
  sf::Font font;
  if (!font.loadFromFile(path)) {
    throw std::runtime_error("Failed to load font: " + path);
  }
  auto [inserted, success] = _fonts.emplace(path, std::move(font));
  (void)success;
  return inserted->second;
}
