add_library(arcade_pacman MODULE pacman.cpp checkCollision.cpp createEntities.cpp moveEntities.cpp map.cpp AI/pathfinder.cpp)

set_target_properties(arcade_pacman PROPERTIES
    PREFIX ""
    SUFFIX ".so"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/lib"
)

target_include_directories(arcade_pacman PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../../../shared)
