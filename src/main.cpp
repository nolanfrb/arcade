#include <cstring>
#include <iostream>
#include "core/core.hpp"

static constexpr const char *HELP_MSG =
    "USAGE\n"
    "\t./arcade path_to_graphical_library\n"
    "\n"
    "DESCRIPTION\n"
    "\tpath_to_graphical_library\tpath to the initial graphical\n"
    "\t\t\t\t\tlibrary to use (e.g. ./lib/arcade_ncurses.so)\n"
    "\n"
    "KEYS\n"
    "\tnext graphics library\n"
    "\tnext game\n"
    "\trestart the game\n"
    "\tgo back to the menu\n"
    "\texit\n";

static constexpr const char *USAGE_MSG =
    "Usage: ./arcade path_to_graphical_library\n";

int main(int argc, char **argv) {
  if (argc == 2 &&
      (std::strcmp(argv[1], "-h") == 0 ||
       std::strcmp(argv[1], "--help") == 0)) {
    std::cout << HELP_MSG;
    return 0;
  }
  if (argc != 2) {
    std::cerr << USAGE_MSG;
    return ERROR;
  }
  Core core;
  return core.run(argv[1]);
}
