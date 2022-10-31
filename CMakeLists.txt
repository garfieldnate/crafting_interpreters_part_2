cmake_minimum_required(VERSION 3.10)
project(clox VERSION 0.0.1)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED True)

configure_file(cloxConfig.h.in cloxConfig.h)

add_executable(clox main.c)

# must include project output directory because it contains generated header files
include_directories(clox PUBLIC "${PROJECT_BINARY_DIR}")

