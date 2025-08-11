cmake_minimum_required(VERSION 3.20)
project(Event LANGUAGES CXX)

add_library(${PROJECT_NAME}Bus_lib INTERFACE)
target_include_directories(${PROJECT_NAME}Bus_lib INTERFACE models/)
