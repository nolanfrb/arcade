/*
** EPITECH PROJECT, 2026
** arcade
** File description:
** core
*/

#pragma once

#include <chrono>
#include <filesystem>
#include <string>
#include "../shared/Input.hpp"
#include "../shared/interface/IDisplay.hpp"
#include "../shared/interface/IGame.hpp"
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

  int initCore(std::filesystem::path const& path);
  void updateGame(IGame* currentGame, IDisplay* currentDisplay, Input input,
                  std::chrono::steady_clock::time_point& lastFrameTime);
  int run(std::filesystem::path const& path);
  void menu();
  void loadGame(std::string const& path);
  void loadDisplay(std::string const& path);

 private:
  bool isInMenu();
  bool handleConfirmQuit(Input input);
  static void appendConfirmTexts(std::vector<Text>& texts);

  bool _running = true;
  bool _confirmingQuit = false;

  SystemCommand _systemCommand;
  LibManager _libManager;
  CoreContext _ctx{_libManager};
};
