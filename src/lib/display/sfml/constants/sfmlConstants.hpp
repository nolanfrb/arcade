/*
** EPITECH PROJECT, 2026
** Arcade
** File description:
** sfmlConstants
*/

#pragma once

namespace sfml {

constexpr float TILE_SIZE = 20.F;
constexpr unsigned int FRAMERATE_LIMIT = 60;
constexpr unsigned int WINDOW_WIDTH = 800;
constexpr unsigned int WINDOW_HEIGHT = 600;
constexpr auto* WINDOW_TITLE = "Arcade";
constexpr auto* DEFAULT_FONT = "assets/fonts/PressStart2P.ttf";

constexpr unsigned int PRINTABLE_MIN = 32;
constexpr unsigned int PRINTABLE_MAX = 126;
constexpr unsigned int BACKSPACE_CODE = 8;

}  // namespace sfml
