#https://pabloariasal.github.io/2018/02/19/its-time-to-do-cmake-right/

cmake_minimum_required(VERSION 3.17)
project(Ludere)

set(CMAKE_CXX_STANDARD 20)

add_library(Ludere src/core/Engine.cpp include/ludere/Engine.hpp)
target_include_directories(Ludere include/)