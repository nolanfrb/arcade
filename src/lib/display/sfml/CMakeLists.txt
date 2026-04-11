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

if(APPLE)
    execute_process(
        COMMAND brew --prefix sfml
        OUTPUT_VARIABLE SFML_BREW_PREFIX
        OUTPUT_STRIP_TRAILING_WHITESPACE
        RESULT_VARIABLE SFML_BREW_RESULT
    )
    if(SFML_BREW_RESULT EQUAL 0)
        list(APPEND CMAKE_PREFIX_PATH "${SFML_BREW_PREFIX}")
    endif()
endif()

find_package(SFML 2.6.1 REQUIRED COMPONENTS graphics window system audio)
target_link_libraries(arcade_sfml PRIVATE sfml-graphics sfml-window sfml-system sfml-audio)
