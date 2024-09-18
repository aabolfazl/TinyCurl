cmake_minimum_required(VERSION 3.28)
project(tcurl C)


set(CMAKE_C_STANDARD 11)

add_subdirectory(libs/arg_parse)

add_executable(tcurl main.c)

target_link_libraries(tcurl PRIVATE argparse)

target_include_directories(tcurl PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/libs/arg_parse)
