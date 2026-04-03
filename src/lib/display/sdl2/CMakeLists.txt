add_library(arcade_sdl2 MODULE SDL2Display.cpp wrapper/SDL2Renderer.cpp)

set_target_properties(arcade_sdl2 PROPERTIES
    PREFIX ""
    SUFFIX ".so"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/lib")

target_include_directories(arcade_sdl2 PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../../../shared)

find_package(SDL2 REQUIRED)

# Apple Silicon fix
if(APPLE)
    if(EXISTS /opt/homebrew)
        set(HOMEBREW_PREFIX /opt/homebrew)
    else()
        set(HOMEBREW_PREFIX /usr/local)
    endif()

    find_library(SDL2_IMAGE_LIBRARY
        NAMES SDL2_image
        HINTS ${HOMEBREW_PREFIX}/lib
        REQUIRED
    )

    find_library(SDL2_TTF_LIBRARY
        NAMES SDL2_ttf
        HINTS ${HOMEBREW_PREFIX}/lib
        REQUIRED
    )

    target_include_directories(arcade_sdl2 PRIVATE
        ${SDL2_INCLUDE_DIRS}
        ${HOMEBREW_PREFIX}/include
    )

    target_link_libraries(arcade_sdl2 PRIVATE
        ${SDL2_LIBRARIES}
        ${SDL2_IMAGE_LIBRARY}
        ${SDL2_TTF_LIBRARY}
    )
else()
    find_package(SDL2_image REQUIRED)
    find_package(SDL2_ttf REQUIRED)

    target_include_directories(arcade_sdl2 PRIVATE
        ${SDL2_INCLUDE_DIRS}
        ${SDL2_IMAGE_INCLUDE_DIRS}
        ${SDL2_TTF_INCLUDE_DIRS}
    )

    target_link_libraries(arcade_sdl2 PRIVATE
        ${SDL2_LIBRARIES}
        SDL2_image
        SDL2_ttf
    )
endif()
