cmake_minimum_required(VERSION 3.28)
project(tcurl C)

set(CMAKE_C_STANDARD 11)

add_subdirectory(libs/arg_parse)

add_executable(tcurl src/main.c src/socket.c
        src/socket.h
        src/send_http_request.c
        src/send_http_request.h)

target_link_libraries(tcurl PRIVATE argparse)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)


target_include_directories(tcurl PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/libs/arg_parse)
