cmake_minimum_required(VERSION 3.28)

project(arcade LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(NOT CMAKE_RUNTIME_OUTPUT_DIRECTORY)
  set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})
endif()

add_subdirectory(src)
add_subdirectory(lib)

option(WITH_TESTS "Build unit tests" OFF)

if (WITH_TESTS)
  enable_testing()
  add_subdirectory(tests)
endif()
