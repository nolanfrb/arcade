/*
** EPITECH PROJECT, 2026
** arcade
** File description:
** dlLoader
*/

#pragma once

#include <dlfcn.h>
#include <filesystem>
#include <string>

template <typename T>
class dlLoader {
 public:
  dlLoader();
  ~dlLoader();

  dlLoader(const dlLoader& other) = delete;
  dlLoader& operator=(const dlLoader& other) = delete;
  dlLoader(dlLoader&& other) = delete;
  dlLoader& operator=(dlLoader&& other) = delete;

  void loadLib(std::filesystem::path const& path);
  T* getInstance(std::string const& entryPoint);
  void unloadLib();

  static bool hasSymbol(std::filesystem::path const& path,
                        std::string const& symbol) {
    void* lib = dlopen(path.c_str(), RTLD_LAZY);
    if (lib == nullptr) {
      return false;
    }
    const bool found = (dlsym(lib, symbol.c_str()) != nullptr);
    dlclose(lib);
    return found;
  }

 private:
  void* handle = nullptr;
};
