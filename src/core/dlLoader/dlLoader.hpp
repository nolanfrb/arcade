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
class DlLoader {
 public:
  DlLoader();
  ~DlLoader();

  DlLoader(const DlLoader& other) = delete;
  DlLoader& operator=(const DlLoader& other) = delete;
  DlLoader(DlLoader&& other) = delete;
  DlLoader& operator=(DlLoader&& other) = delete;

  void loadLib(std::filesystem::path const& path);
  T* getInstance(std::string const& entryPoint);
  void unloadLib();

  static bool hasSymbol(std::filesystem::path const& path,
                        std::string const& symbol) {
    void* lib = dlopen(path.c_str(), RTLD_LAZY);
    if (lib == nullptr) {
      return false;
    }

    // NOLINTNEXTLINE(concurrency-mt-unsafe)
    dlerror();
    void* symbolAddress = dlsym(lib, symbol.c_str());
    // NOLINTNEXTLINE(concurrency-mt-unsafe)
    const bool found = (dlerror() == nullptr) && (symbolAddress != nullptr);

    dlclose(lib);
    return found;
  }

 private:
  void* handle = nullptr;
};
