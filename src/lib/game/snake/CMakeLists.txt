add_library(arcade_snake MODULE snake.cpp)

set_target_properties(arcade_snake PROPERTIES
    PREFIX ""
    SUFFIX ".so"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/lib")

target_include_directories(arcade_snake PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../../../shared)

set_target_properties(arcade_snake PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED ON)

if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    target_compile_options(arcade_snake PRIVATE
        -Wall -Wextra -Werror
        -Wno-zero-as-null-pointer-constant
        -Wno-padded)
endif()
