cmake_minimum_required(VERSION 2.8)
project(Tau-project)

set(CMAKE_CXX_STANDARD 11)
add_definitions(-std=c++11)

add_executable(Tau-project main.cpp parser_main.h parser_main.cpp worker_manager_worker_object.h worker_manager_worker_object.cpp worker_manager.h worker_manager.cpp)
