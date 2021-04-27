cmake_minimum_required(VERSION 3.15)
project(concurrent-counter C)
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -pthread")

add_executable(concurrent-counter test.c counter.h counter.h)