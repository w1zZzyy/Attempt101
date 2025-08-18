cmake_minimum_required(VERSION 3.20)
project(Attempt)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -march=native -flto")

add_subdirectory(libs)
add_subdirectory(desktop)