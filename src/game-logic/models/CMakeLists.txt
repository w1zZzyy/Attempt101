cmake_minimum_required(VERSION 3.20)

project(Model LANGUAGES CXX) 

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDART_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)


find_package(Boost REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})


add_library(${PROJECT_NAME}Message_lib STATIC 
    request/view.hpp request/view.cpp
    response/view.hpp response/view.cpp
)
target_link_libraries(${PROJECT_NAME}Player_lib PUBLIC ${Boost_LIBRARIES})


add_library(${PROJECT_NAME}Player_lib STATIC player/view.hpp player/view.cpp)
target_link_libraries(${PROJECT_NAME}Player_lib PUBLIC ${Boost_LIBRARIES})


add_library(${PROJECT_NAME}Timer_lib STATIC timer/view.hpp timer/view.cpp)
target_link_libraries(${PROJECT_NAME}Timer_lib PUBLIC ${Boost_LIBRARIES})


add_library(${PROJECT_NAME}Match_lib STATIC match/view.hpp match/view.cpp)
target_link_libraries(${PROJECT_NAME}Match_lib PUBLIC 
    ${PROJECT_NAME}Logic_lib
    ${PROJECT_NAME}Player_lib 
    ${PROJECT_NAME}Message_lib
    ${PROJECT_NAME}Timer_lib
)