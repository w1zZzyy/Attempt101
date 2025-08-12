cmake_minimum_required(VERSION 3.20)
project(Controller LANGUAGES CXX)

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/../
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../libs/
)

add_library(${PROJECT_NAME}Game_lib STATIC game/view.hpp game/view.cpp)
target_link_libraries(${PROJECT_NAME}Game_lib PUBLIC EventBus_lib LogicManager_lib SFML::Graphics)

add_library(${PROJECT_NAME}UI_lib STATIC 
    ui/board/view.hpp 
    ui/board/view.cpp
)
target_link_libraries(${PROJECT_NAME}UI_lib PUBLIC EventBus_lib UI_lib)

add_library(${PROJECT_NAME}Input_lib STATIC input/view.hpp input/view.cpp)
target_link_libraries(${PROJECT_NAME}Input_lib PUBLIC EventBus_lib SFML::Graphics)