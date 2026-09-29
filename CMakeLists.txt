cmake_minimum_required(VERSION 3.16)
project(cpp-todo LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(todo src/main.cpp)

if(MSVC)
    target_compile_options(todo PRIVATE /W4)
else()
    target_compile_options(todo PRIVATE -Wall -Wextra)
endif()