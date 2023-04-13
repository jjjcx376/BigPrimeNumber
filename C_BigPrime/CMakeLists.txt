cmake_minimum_required(VERSION 3.22)
project(MillerRabin C)

set(CMAKE_C_STANDARD 11)

include_directories(.)

add_executable(MillerRabin
        1.c
        C.millerrabin.h
        C.operate.h)
