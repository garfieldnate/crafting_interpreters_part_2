cmake_minimum_required(VERSION 3.24)
project(clox VERSION 0.0.1)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED True)

include(CTest)
enable_testing()

option(DEBUG_PRINT_CODE "print out bytecode before execution" OFF)
if(DEBUG_PRINT_CODE)
    add_definitions(-DDEBUG_PRINT_CODE)
endif(DEBUG_PRINT_CODE)

option(DEBUG_TRACE_EXECUTION "print the stack and dissassembled bytecode during execution" OFF)
if(DEBUG_TRACE_EXECUTION)
    add_definitions(-DDEBUG_TRACE_EXECUTION)
endif(DEBUG_TRACE_EXECUTION)

configure_file(src/cloxConfig.h.in cloxConfig.h)

add_executable(clox src/main.c src/chunk.c src/compiler.c src/debug.c src/memory.c src/object.c src/scanner.c src/value.c src/vm.c src/table.c)

# must include project output directory because it contains generated header files
target_include_directories(clox PUBLIC "${PROJECT_BINARY_DIR}")

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
