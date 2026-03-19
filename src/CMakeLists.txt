add_executable(arcade
  main.cpp
)

target_include_directories(arcade PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/shared
)

target_compile_options(arcade PRIVATE
  -Wall -Wextra -Werror
)
