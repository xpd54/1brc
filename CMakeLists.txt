cmake_minimum_required(VERSION 3.26)
project(1brc)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_BUILD_TYPE "Release")

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

add_compile_options("-O3;-march=native;-fomit-frame-pointer;")

add_subdirectory(src/level_0)
add_subdirectory(src/level_1)
add_subdirectory(src/level_2)
add_subdirectory(src/level_3)