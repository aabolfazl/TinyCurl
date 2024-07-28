cmake_minimum_required(VERSION 3.0)
project(argparse VERSION 0.1.0 LANGUAGES C)

set(CMAKE_C_STANDARD 11)

# Set compiler flags
if (NOT CMAKE_C_FLAGS)
    set(CMAKE_C_FLAGS "-O3")
endif ()
if (NOT CMAKE_C_FLAGS_DEBUG)
    set(CMAKE_C_FLAGS_DEBUG "-g -ggdb")
endif ()

# Create the static library
add_library(argparse STATIC arg_parse.c)

# Set include directories
target_include_directories(argparse PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})