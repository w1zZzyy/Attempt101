cmake_minimum_required(VERSION 3.20)
project(Scene LANGUAGES CXX)

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/../
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../libs/
)

add_library(${PROJECT_NAME}Game_lib STATIC game/view.hpp game/view.cpp)
target_link_libraries(${PROJECT_NAME}Game_lib PUBLIC 
    ResourceConfig_lib 
    ControllerGame_lib 
    ControllerUI_lib
    ControllerInput_lib
)

add_library(${PROJECT_NAME}Manager_lib STATIC manager/view.hpp manager/view.cpp)
target_link_libraries(${PROJECT_NAME}Manager_lib PUBLIC ${PROJECT_NAME}Game_lib)