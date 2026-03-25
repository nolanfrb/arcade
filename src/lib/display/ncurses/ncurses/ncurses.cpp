/*
** EPITECH PROJECT, 2026
** arcade
** File description:
** ncurses
*/

#include "ncurses.hpp"
#include <ncurses.h>

void ncurses::init() {
  initscr();
  cbreak();
  noecho();
  keypad(stdscr, TRUE);
  timeout(16);
}
