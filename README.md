# Crafting Interpreters, Part 2

This is my repository for working through the code and exercises in Robert Nystrom's excellen book, [Crafting Interpreters](https://craftinginterpreters.com/a-bytecode-virtual-machine.html).
When complete, this project will contain a bytecode interpreter (VM) for the Lox language, written in C and known as `clox`.

My repo for part 1 of the book can be found [here](https://github.com/garfieldnate/crafting_interpreters_part1).

The project is licensed under MIT, as is the code from the book (which, of course, makes up most of this project).

## Building

First, configure the project (generate the build directory):

    cmake -S . -B build

To enable one of the debug options specified in `CMakeLists.txt`, use the `-D` flag:

    cmake -S . -B build -DDEBUG_PRINT_CODE=ON

Next, build the project:

    cmake --build build

## Running

Once built, you can run the resulting executable with:

    build/clox [file.lox]

If no Lox source file is provided, the REPL will be started.

## TODO

