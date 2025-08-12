cmake_minimum_required(VERSION 3.20)
project(Event LANGUAGES CXX)

add_library(${PROJECT_NAME}Bus_lib STATIC bus/view.hpp bus/view.cpp models/event_type.hpp)
target_link_libraries(${PROJECT_NAME}Bus_lib PUBLIC SFML::Graphics Logic_lib)
target_include_directories(${PROJECT_NAME}Bus_lib PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../libs/
)