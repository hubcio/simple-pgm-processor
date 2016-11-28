cmake_minimum_required(VERSION 3.6)
project(simple_pgm_processor)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c11 -Werror -Wall")

set(SOURCE_FILES
        main.c
        main.h)

add_executable(simple_pgm_processor ${SOURCE_FILES})