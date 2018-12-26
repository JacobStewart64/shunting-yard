cmake_minimum_required(VERSION 3.10.2)

project(parsemath)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(parsemath shunting_yard.cpp)


