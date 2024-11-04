cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

project(Cmax)

add_executable(Cmax main.cpp) 

target_include_directories(Cmax PUBLIC "include") 