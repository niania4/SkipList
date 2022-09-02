cmake_minimum_required(VERSION 3.0)

project(SkipList)

add_compile_options(-Wall -std=c++11)

include_directories(./include)

add_executable(main_cmake strees_test.cpp)