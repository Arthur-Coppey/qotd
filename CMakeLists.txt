cmake_minimum_required(VERSION 3.19)
project(qotd C)

set(CMAKE_C_STANDARD 99)

set(CMAKE_THREAD_PREFER_PTHREAD TRUE)
set(THREADS_PREFER_PTHREAD_FLAG TRUE)
find_package(Threads REQUIRED)

add_executable(qotd main.c quote.c quote.h server.c server.h config.h)
target_link_libraries(qotd Threads::Threads)