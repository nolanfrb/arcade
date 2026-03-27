add_executable(arcade
  main.cpp
)

add_subdirectory(core)
add_subdirectory(lib)

target_link_libraries(arcade PRIVATE core_lib)

target_include_directories(arcade PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/shared
)

target_compile_options(arcade PRIVATE
  -Wall -Wextra -Werror
)
