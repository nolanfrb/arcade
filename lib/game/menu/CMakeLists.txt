add_library(arcade_menu SHARED
    menu.cpp
)

target_include_directories(arcade_menu PRIVATE
    ${CMAKE_SOURCE_DIR}/src/shared
)

set_target_properties(arcade_menu PROPERTIES
    PREFIX ""  # évite le "lib" devant → arcade_menu.so au lieu de libarcade_menu.so
)
