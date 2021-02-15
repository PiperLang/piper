cmake_minimum_required(VERSION 3.13)
project(piper)

add_executable(piper src/piper.cpp src/scanner.cpp)
target_include_directories(piper PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src/includes)