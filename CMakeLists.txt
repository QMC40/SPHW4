cmake_minimum_required(VERSION 3.21)
project(vers1 C)

set(CMAKE_C_STANDARD 11)

add_executable(vers1 main.c copier.h tlpi_hdr.h error_functions.h error_functions.c sort.h medianator.h)
