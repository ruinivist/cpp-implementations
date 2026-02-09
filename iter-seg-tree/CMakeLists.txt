cmake_minimum_required(VERSION 4.0)

project(iter_seg_tree)
set(CMAKE_CXX_STANDARD 20)

add_executable(iter_seg_tree src/main.cpp)

target_include_directories(iter_seg_tree PRIVATE ${CMAKE_SOURCE_DIR}/include)