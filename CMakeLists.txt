cmake_minimum_required(VERSION 3.28)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(SRC)

project(arcade)

if(NOT CMAKE_RUNTIME_OUTPUT_DIRECTORY)
  set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})
endif()

add_executable(arcade ${SRC})

target_include_directories(arcade PRIVATE ${CMAKE_SOURCE_DIR}/include)
target_compile_features(arcade PRIVATE cxx_std_20)
target_compile_options(arcade PRIVATE
  -Wall -Wextra -Werror
)
option(WITH_TESTS "Build unit tests" OFF)

if (WITH_TESTS)
  enable_testing()
  add_subdirectory(tests)
endif()
