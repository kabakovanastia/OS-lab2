cmake_minimum_required(VERSION 3.10)
project(CrossPlatformProcess LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_library(process_manager STATIC process_manager.cpp)

add_executable(test_process main.cpp)

target_link_libraries(test_process PRIVATE process_manager)