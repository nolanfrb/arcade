add_library(arcade_sdl2 MODULE SDL2Display.cpp wrapper/SDL2Renderer.cpp)

set_target_properties(arcade_sdl2 PROPERTIES
    PREFIX ""
    SUFFIX ".so"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/lib")

target_include_directories(arcade_sdl2 PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../../../shared)

find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)

target_include_directories(arcade_sdl2 PRIVATE ${SDL2_INCLUDE_DIRS} ${SDL2_IMAGE_INCLUDE_DIRS})

target_link_libraries(arcade_sdl2 PRIVATE ${SDL2_LIBRARIES} SDL2_image)
