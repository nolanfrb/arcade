#include <iostream>
#include "core/core.hpp"

int main(int argc, char** argv) {
  if (argc != 2) {
    std::cerr << "Usage: " << argv[0] << " <path>" << '\n';
    return ERROR;
  }
  core core;
  return core.run(argv[1]);
}
