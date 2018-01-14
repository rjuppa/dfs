cmake_minimum_required(VERSION 3.8)
project(dfs)

set(CMAKE_C_STANDARD 99)

set(SOURCE_FILES main.c graph.c graph.h stack.c stack.h)
add_executable(dfs ${SOURCE_FILES})