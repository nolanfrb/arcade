/*
** EPITECH PROJECT, 2026
** arcade
** File description:
** dlLoader
*/

#include "dlLoader.hpp"
#include <dlfcn.h>
#include <iostream>
#include <bit>
#include "../../shared/interface/IDisplay.hpp"
#include "../../shared/interface/IGame.hpp"

template <typename T>
void dlLoader<T>::loadLib(std::filesystem::path const& path) {
  handle = dlopen(path.c_str(), RTLD_LAZY);
  if (handle == nullptr) {
    std::cerr << "dlopen failed: " << "\n";
  }
}

template <typename T>
T* dlLoader<T>::getInstance(std::string const& entryPoint) {
  void* ptr = dlsym(handle, entryPoint.c_str());
  if (ptr == nullptr) {
    return nullptr;
  }
  auto createFunc = std::bit_cast<T* (*)()>(ptr);
  return createFunc();
}

template <typename T>
void dlLoader<T>::unloadLib() {
  dlclose(handle);
}

template class dlLoader<IGame>;
template class dlLoader<IDisplay>;
