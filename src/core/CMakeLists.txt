add_library(core_lib STATIC
    core.cpp
    LibManager/libManager.cpp
    SystemCommand/systemCommand.cpp
    dlLoader/dlLoader.cpp
)
target_include_directories(core_lib PUBLIC .)

target_include_directories(core_lib PRIVATE ../shared)

target_link_libraries(core_lib PRIVATE ${CMAKE_DL_LIBS})
