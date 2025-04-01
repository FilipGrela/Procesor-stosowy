cmake_minimum_required(VERSION 3.30)
project(Procesor_stosowy)

set(CMAKE_CXX_STANDARD 20)

add_executable(Procesor_stosowy main.cpp
        stack.cpp
        stack.h)
