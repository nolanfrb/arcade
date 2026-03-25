find_package(SDL2 REQUIRED)
find_package(SDL2_ttf REQUIRED)
find_package(SDL2_image REQUIRED)

add_library(arcade_sdl2 SHARED
  SDL2Display.cpp
)

target_include_directories(arcade_sdl2 PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/../../shared
  ${SDL2_INCLUDE_DIRS}
)

target_link_libraries(arcade_sdl2 PRIVATE
  ${SDL2_LIBRARIES}
  SDL2_ttf
  SDL2_image
)

target_compile_options(arcade_sdl2 PRIVATE
  -Wall -Wextra -Werror
  -fno-gnu-unique
  -fPIC
)

set_target_properties(arcade_sdl2 PROPERTIES
  LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib
  PREFIX ""
)
