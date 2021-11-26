cmake_minimum_required(VERSION 3.19)
project(qotd C)

set(CMAKE_C_STANDARD 99)

add_executable(qotd main.c quote.c quote.h server.c server.h config.h)