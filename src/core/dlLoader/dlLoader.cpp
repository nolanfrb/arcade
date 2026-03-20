/*
** EPITECH PROJECT, 2026
** arcade
** File description:
** dlLoader
*/

#include "dlLoader.hpp"
#include <dlfcn.h>
#include <iostream>

template <typename T>
void dlLoader<T>::loadLib(std::filesystem::path const& path) {
  handle = dlopen(path.c_str(), RTLD_LAZY);
  if (handle == nullptr) {
    std::cerr << "dlopen failed: " << "\n";
  }
}

template <typename T>
T* dlLoader<T>::getInstance(std::string const& entryPoint) {
  return dlsym(handle, entryPoint.c_str());
}

template <typename T>
void dlLoader<T>::unloadLib() {
  dlclose(handle);
}
