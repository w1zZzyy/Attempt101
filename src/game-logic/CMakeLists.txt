cmake_minimum_required(VERSION 3.20)

project(GameLogic LANGUAGES CXX) 

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDART_REQUIRED ON)

add_subdirectory(logic)
add_subdirectory(models)