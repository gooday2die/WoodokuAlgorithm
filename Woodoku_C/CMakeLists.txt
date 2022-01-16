cmake_minimum_required(VERSION 3.21)
project(WoodokuAI)

set(CMAKE_CXX_STANDARD 14)

add_executable(WoodokuAI main.cpp Shape.cpp Shape.h Field.h Field.cpp Algorithm.h Algorithm.cpp)
