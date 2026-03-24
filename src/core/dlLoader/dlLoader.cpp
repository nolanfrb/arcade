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
DlLoader<T>::DlLoader() = default;

template <typename T>
DlLoader<T>::~DlLoader() {
  if (handle != nullptr) {
    dlclose(handle);
  }
}

template <typename T>
void DlLoader<T>::loadLib(std::filesystem::path const& path) {
  unloadLib();
  handle = dlopen(path.c_str(), RTLD_LAZY);
  if (handle == nullptr) {
    std::cerr << "dlopen failed: " << "\n";
  }
}

template <typename T>
T* DlLoader<T>::getInstance(std::string const& entryPoint) {
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
void DlLoader<T>::unloadLib() {
  if (handle != nullptr) {
    if (dlclose(handle) != 0) {
      std::cerr << "dlclose failed" << "\n";
    }
    handle = nullptr;
  }
}

template class DlLoader<IGame>;
template class DlLoader<IDisplay>;
