cmake_minimum_required(VERSION 3.20)

project(GameLogic LANGUAGES CXX) 

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Boost REQUIRED)
include_directories(handlers models logic)

add_subdirectory(models)
add_subdirectory(handlers)