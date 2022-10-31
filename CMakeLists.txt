cmake_minimum_required(VERSION 3.24)
project(clox VERSION 0.0.1)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED True)

configure_file(src/cloxConfig.h.in cloxConfig.h)

add_executable(clox src/main.c src/chunk.c src/debug.c src/memory.c src/value.c)

# must include project output directory because it contains generated header files
target_include_directories(clox PUBLIC "${PROJECT_BINARY_DIR}")

