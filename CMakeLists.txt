cmake_minimum_required(VERSION 3.19)
project(stenography_shared_secret C)

set(CMAKE_C_STANDARD 99)

add_executable(stenography_shared_secret main.c src/galois.c include/galois.h src/galois_lib.c include/galois_lib.h)