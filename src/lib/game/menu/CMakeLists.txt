add_library(arcade_menu MODULE menu.cpp)

set_target_properties(arcade_menu PROPERTIES
    PREFIX ""
    SUFFIX ".so"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/lib"
)

target_include_directories(arcade_menu PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../../../shared)
