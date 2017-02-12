cmake_minimum_required(VERSION 3.7)
project(lecture_3)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES main.cpp tic_tac_toe.cpp tic_tac_toe.h)
add_executable(lecture_3 ${SOURCE_FILES})