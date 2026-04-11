/*
** EPITECH PROJECT, 2026
** arcade
** File description:
** Registry - holds player data (username, scores)
*/

#pragma once

#include <string>
#include <unordered_map>

class Registry {
 public:
  Registry() = default;
  ~Registry() = default;

  Registry(const Registry&) = delete;
  Registry& operator=(const Registry&) = delete;
  Registry(Registry&&) = delete;
  Registry& operator=(Registry&&) = delete;

  [[nodiscard]] const std::string& getUsername() const { return _username; }
  void setUsername(const std::string& username) { _username = username; }

  [[nodiscard]] int getHighScore(const std::string& gameName) const {
    auto iter = _highScores.find(gameName);
    if (iter != _highScores.end()) {
      return iter->second;
    }
    return 0;
  }

  void setHighScore(const std::string& gameName, int score) {
    if (score > getHighScore(gameName)) {
      _highScores[gameName] = score;
    }
  }

  [[nodiscard]] const std::unordered_map<std::string, int>& getHighScores()
      const {
    return _highScores;
  }

 private:
  std::string _username;
  std::unordered_map<std::string, int> _highScores;
};
