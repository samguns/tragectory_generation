cmake_minimum_required(VERSION 3.12)
project(tragetory_generation)

set(CMAKE_CXX_STANDARD 11)
find_package (Eigen3 3.3 REQUIRED NO_MODULE)

add_executable(tragetory_generation main.cpp hybrid_breadth_first.cpp hybrid_breadth_first.h polynomial_solver.cpp polynomial_solver.h)
target_link_libraries (${PROJECT_NAME} Eigen3::Eigen)