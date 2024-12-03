cmake_minimum_required(VERSION 3.26)
project(InputGenerator)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_BUILD_TYPE "Release")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_compile_options("$<$<COMPILE_LANGUAGE:CXX>:-Ofast;-DNDEBUG;-march=native;-fpic;-ftree-vectorize>")
add_executable(${PROJECT_NAME} main.cpp)