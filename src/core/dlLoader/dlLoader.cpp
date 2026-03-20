/*
** EPITECH PROJECT, 2026
** arcade
** File description:
** dlLoader
*/

#include "dlLoader.hpp"
#include <dlfcn.h>
#include <filesystem>
#include <iostream>
#include <string>
#include "../../shared/interface/IDisplay.hpp"
#include "../../shared/interface/IGame.hpp"

template <typename T>
dlLoader<T>::dlLoader() = default;

template <typename T>
dlLoader<T>::~dlLoader() {
  if (handle != nullptr) {
    dlclose(handle);
  }
}

template <typename T>
void dlLoader<T>::loadLib(std::filesystem::path const& path) {
  unloadLib();
  handle = dlopen(path.c_str(), RTLD_LAZY);
  if (handle == nullptr) {
    std::cerr << "dlopen failed: " << "\n";
  }
}

template <typename T>
T* dlLoader<T>::getInstance(std::string const& entryPoint) {
  if (handle == nullptr) {
    return nullptr;
  }
  void* ptr = dlsym(handle, entryPoint.c_str());
  if (ptr == nullptr) {
    return nullptr;
  }
  // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
  auto createFunc = reinterpret_cast<T* (*)()>(ptr);
  return createFunc();
}

template <typename T>
void dlLoader<T>::unloadLib() {
  if (handle != nullptr) {
    if (dlclose(handle) != 0) {
      std::cerr << "dlclose failed" << "\n";
    }
    handle = nullptr;
  }
}

template class dlLoader<IGame>;
template class dlLoader<IDisplay>;
