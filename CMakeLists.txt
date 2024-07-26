cmake_minimum_required(VERSION 3.28)
project(tiny_curl C)

set(CMAKE_C_STANDARD 11)

add_executable(tiny_curl main.c
        arg_parse/arg_parse.c)
