/* ------------------------------------------------------------------------------------
 * *
 *                                                                                      *
 * EPITECH PROJECT - Thu, Mar, 2026 * Title           - arcade * Description - *
 *     IGame *
 *                                                                                      *
 * ------------------------------------------------------------------------------------
 * *
 *                                                                                      *
 *         ░        ░       ░░        ░        ░        ░░      ░░  ░░░░  ░ * ▒
 * ▒▒▒▒▒▒▒  ▒▒▒▒  ▒▒▒▒  ▒▒▒▒▒▒▒  ▒▒▒▒  ▒▒▒▒▒▒▒  ▒▒▒▒  ▒  ▒▒▒▒  ▒             *
 *         ▓      ▓▓▓       ▓▓▓▓▓  ▓▓▓▓▓▓▓  ▓▓▓▓      ▓▓▓  ▓▓▓▓▓▓▓        ▓ * █
 * ███████  ██████████  ███████  ████  ███████  ████  █  ████  █             *
 *         █        █  ███████        ████  ████        ██      ██  ████  █ *
 *                                                                                      *
 * ------------------------------------------------------------------------------------
 */

#ifndef INCLUDED_IGAME_HPP
#define INCLUDED_IGAME_HPP

#include <string>
#include <vector>
#include "../Entity.hpp"
#include "../Input.hpp"
#include "../Text.hpp"

class IGame {
 public:
  virtual ~IGame() = default;

  IGame(const IGame&) = default;
  IGame& operator=(const IGame&) = default;
  IGame(IGame&&) = default;
  IGame& operator=(IGame&&) = default;

  virtual void init() = 0;
  virtual void stop() = 0;
  virtual void restart() = 0;

  virtual void update(Input input) = 0;

  virtual std::vector<Entity> getEntity() = 0;
  virtual std::vector<Text> getText() = 0;

  virtual int getScore() = 0;
  virtual bool isGameOver() = 0;
  virtual std::string getName() = 0;
};

#endif
