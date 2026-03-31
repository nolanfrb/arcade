add_library(arcade_sfml MODULE
    sfmlDisplay.cpp
    wrapper/sfmlRenderer.cpp
)

set_target_properties(arcade_sfml PROPERTIES
    PREFIX ""
    SUFFIX ".so"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/lib"
)

target_include_directories(arcade_sfml PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../../../shared)

find_package(SFML 2.6.1 REQUIRED COMPONENTS graphics window system)
target_link_libraries(arcade_sfml PRIVATE sfml-graphics sfml-window sfml-system)
