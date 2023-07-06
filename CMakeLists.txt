cmake_minimum_required(VERSION 3.15)

# project settings
project(nes_emu LANGUAGES CXX)

# c/c++ standards
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# sources
set(
    project_source_files
    # source files go here
    "source/main.cpp"
    "source/nes/cpu.cpp"
    "source/nes/memory.cpp"
    "source/nes/bus.cpp"
    "source/nes/system.cpp"
    "source/nes/rom.cpp"
)

include(FetchContent)

# fix some timestamp warnings...
cmake_policy(SET CMP0135 NEW)

# fmt
FetchContent_Declare(fmt URL "https://github.com/fmtlib/fmt/releases/download/10.0.0/fmt-10.0.0.zip")
FetchContent_MakeAvailable(fmt)

# raylib
FetchContent_Declare(raylib URL "https://github.com/raysan5/raylib/archive/refs/tags/4.5.0.zip")
FetchContent_MakeAvailable(raylib)

# setup link directory
link_directories("${CMAKE_SOURCE_DIR}/external/libs")

# executable
add_executable(${PROJECT_NAME} ${project_source_files})

# include directory
target_include_directories(${PROJECT_NAME} PRIVATE "${CMAKE_SOURCE_DIR}/include/")

# link libraries
target_link_libraries(${PROJECT_NAME} PRIVATE WinMM)
target_link_libraries(${PROJECT_NAME} PRIVATE raylib)
target_link_libraries(${PROJECT_NAME} PRIVATE fmt::fmt)

# copy assets to binary directory
add_custom_command(
    TARGET ${PROJECT_NAME}
    POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory
    "${PROJECT_SOURCE_DIR}/assets" $<TARGET_FILE_DIR:${PROJECT_NAME}>/assets
)