/*
** EPITECH PROJECT, 2026
** arcade
** File description:
** core
*/

#pragma once

#include <filesystem>
#include <string>
#include "CoreContext.hpp"
#include "LibManager/libManager.hpp"
#include "SystemCommand/systemCommand.hpp"

constexpr int ERROR = 84;
constexpr float DEFAULT_DELTA_TIME = 0.016F;
constexpr float MAX_DELTA_TIME = 0.1F;

class Core {
 public:
  Core();
  ~Core() = default;

  Core(const Core& other) = delete;
  Core& operator=(const Core& other) = delete;
  Core(Core&& other) = delete;
  Core& operator=(Core&& other) = delete;

  int run(std::filesystem::path const& path);
  void menu();
  void loadGame(std::string const& path);
  void loadDisplay(std::string const& path);

 private:
  bool _running = true;

  SystemCommand _systemCommand;
  LibManager _libManager;
  CoreContext _ctx{_libManager};
};
