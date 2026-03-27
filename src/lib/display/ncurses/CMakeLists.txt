add_library(arcade_ncurses MODULE
    ncursesDisplay.cpp
    ncurses/ncurses.cpp
)

set_target_properties(arcade_ncurses PROPERTIES
    PREFIX ""
    SUFFIX ".so"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/lib"
)

target_include_directories(arcade_ncurses PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../../../shared)

find_package(Curses REQUIRED)
target_include_directories(arcade_ncurses PRIVATE ${CURSES_INCLUDE_DIRS})
target_link_libraries(arcade_ncurses PRIVATE ${CURSES_LIBRARIES})
