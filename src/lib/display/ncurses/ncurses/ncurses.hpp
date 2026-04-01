/*
** EPITECH PROJECT, 2026
** arcade
** File description:
** ncurses
*/

#pragma once

#include <ncurses.h>

class Ncurses {
 public:
  Ncurses() = default;
  ~Ncurses() = default;

  Ncurses(const Ncurses&) = delete;
  Ncurses& operator=(const Ncurses&) = delete;
  Ncurses(Ncurses&&) = delete;
  Ncurses& operator=(Ncurses&&) = delete;

  static void init();
  static void stop() { endwin(); }
  static void clear() { ::clear(); }
  static int readInput() { return ::getch(); }
  static void putItem(int coordy, int coordx, char character) {
    mvaddch(coordy, coordx, character);
  }
};
