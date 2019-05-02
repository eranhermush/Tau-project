cmake_minimum_required(VERSION 2.8)
project(Tau-project-Worker)

set(CMAKE_CXX_STANDARD 11)
add_definitions(-std=c++11)

add_executable(Worker_run main.cpp worker.h worker.cpp scheme.h scheme.cpp)
