cmake_minimum_required(VERSION 3.15)
project(concurrent-counter C)
set(CMAKE_C_STANDARD 99)

add_executable(concurrent-counter test.c counter.h counter.h)