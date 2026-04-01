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

namespace {
void logDlError(const std::string& context) {
  // NOLINTNEXTLINE(concurrency-mt-unsafe)
  const char* error = dlerror();

  if (error != nullptr) {
    std::cerr << context << ": " << error << "\n";
  } else {
    std::cerr << context << ": unknown dynamic loader error\n";
  }
}
}  // namespace

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
  // NOLINTNEXTLINE(concurrency-mt-unsafe)
  dlerror();
  handle = dlopen(path.c_str(), RTLD_LAZY);
  if (handle == nullptr) {
    logDlError("dlopen failed");
  }
}

template <typename T>
T* DlLoader<T>::getInstance(std::string const& entryPoint) {
  if (handle == nullptr) {
    return nullptr;
  }

  // NOLINTNEXTLINE(concurrency-mt-unsafe)
  dlerror();
  void* ptr = dlsym(handle, entryPoint.c_str());
  // NOLINTNEXTLINE(concurrency-mt-unsafe)
  const char* error = dlerror();

  if (error != nullptr) {
    std::cerr << "dlsym failed: " << error << "\n";
    return nullptr;
  }

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
      logDlError("dlclose failed");
    }
    handle = nullptr;
  }
}

template class DlLoader<IGame>;
template class DlLoader<IDisplay>;
