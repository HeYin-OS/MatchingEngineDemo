# Cmake Version
cmake_minimum_required(VERSION 3.10)

# Project Name
project(OrderMatchingEngine)

# CPP Standard Version
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(include)

file(GLOB SOURCES "src/*.cpp")

add_executable(OrderMatchingEngine ${SOURCES})

if(MSVC)
    add_compile_options(/W4)
else()
    add_compile_options(-Wall -Wextra)
endif()
