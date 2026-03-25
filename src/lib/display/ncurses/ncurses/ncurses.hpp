/*
** EPITECH PROJECT, 2026
** arcade
** File description:
** ncurses
*/

#pragma once

#include <ncurses.h>

class ncurses {
 public:
  ncurses();
  ~ncurses();

  ncurses(const ncurses&) = delete;
  ncurses& operator=(const ncurses&) = delete;
  ncurses(ncurses&&) = delete;
  ncurses& operator=(ncurses&&) = delete;

  static void init();
  static void stop() { endwin(); }
  static void clear() { ::clear(); }
  static int readInput() { return ::getch(); }
  static void putItem(int coordy, int coordx, char character) {
    mvaddch(coordy, coordx, character);
  }
};
