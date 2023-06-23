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
    "${CMAKE_SOURCE_DIR}/source/main.cpp"
    "${CMAKE_SOURCE_DIR}/source/nes/cpu.cpp"
    "${CMAKE_SOURCE_DIR}/source/nes/memory.cpp"
    "${CMAKE_SOURCE_DIR}/source/nes/bus.cpp"
)

# setup link directory
link_directories("${CMAKE_SOURCE_DIR}/external/libs")

# executable
add_executable(${PROJECT_NAME} ${project_source_files})

# include directory
target_include_directories(${PROJECT_NAME} PRIVATE "${CMAKE_SOURCE_DIR}/include/")
target_include_directories(${PROJECT_NAME} PRIVATE "${CMAKE_SOURCE_DIR}/external/include/")

# link libraries
target_link_libraries(${PROJECT_NAME} PRIVATE WinMM)
target_link_libraries(${PROJECT_NAME} PRIVATE raylib)
