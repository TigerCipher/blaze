cmake_minimum_required(VERSION 3.27)
project(sandbox)

set(CMAKE_CXX_STANDARD 20)

add_executable(sandbox main.cpp)
target_include_directories(sandbox PUBLIC "../include/")
target_link_libraries(sandbox PRIVATE blaze)