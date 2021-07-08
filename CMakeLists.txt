cmake_minimum_required(VERSION 3.19)
project(qotd C)

set(CMAKE_C_STANDARD 99)

add_executable(server server/main.c server/quote.c server/quote.h server/server.c server/server.h)
add_executable(client client/main.c)