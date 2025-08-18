cmake_minimum_required(VERSION 3.12)
project(libs LANGUAGES CXX)

add_subdirectory(logic)
add_subdirectory(engine)

add_library(LogicManager_lib STATIC manager/view.hpp manager/view.cpp)
target_link_libraries(LogicManager_lib PUBLIC Logic_lib)

