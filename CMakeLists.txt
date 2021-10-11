cmake_minimum_required(VERSION 3.20)
project(IAL\ |\ 1st\ homework C)
set(CMAKE_SOURCE_DIR src/)
set(PROJECT_BINARY_DIR ../bin/)
set(CMAKE_BINARY_DIR ../bin/)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ../bin/)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_COMPILER gcc)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -pedantic -lm -fcommon")

add_executable(c202 src/c202/c202.c)
add_executable(c204 src/c204/c204.c)
add_executable(c206 src/c206/c206.c)
