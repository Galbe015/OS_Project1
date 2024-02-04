 cmake_minimum_required(VERSION 3.27)
project(Programs C)

set(CMAKE_C_STANDARD 11)

add_executable(Programs main.c
        stack.h
        stack.c)
