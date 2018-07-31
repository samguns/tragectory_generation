cmake_minimum_required(VERSION 3.12)
project(tragetory_generation)

set(CMAKE_CXX_STANDARD 11)

add_executable(tragetory_generation main.cpp hybrid_breadth_first.cpp hybrid_breadth_first.h)