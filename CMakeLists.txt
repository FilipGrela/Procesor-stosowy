cmake_minimum_required(VERSION 3.30)
project(Procesor_stosowy)

set(CMAKE_CXX_STANDARD 20)

add_executable(Procesor_stosowy main.cpp
        stack.cpp
        stack.h
        list.cpp
        list.h)

# Define DEBUG variable if not already defined
if(NOT DEFINED DEBUG)
    set(DEBUG 0)
endif()

# Add DEBUG definition to compiler flags
if(DEBUG)
    add_definitions(-DDEBUG)
endif()