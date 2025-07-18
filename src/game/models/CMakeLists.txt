cmake_minimum_required(VERSION 3.20)

project(Model LANGUAGES CXX) 

add_library(${PROJECT_NAME}Player_lib STATIC player/view.cpp)
target_link_libraries(${PROJECT_NAME}Player_lib PRIVATE ${Boost_LIBRARIES})
target_include_directories(${PROJECT_NAME}Player_lib PRIVATE ${Boost_INCLUDE_DIRS})


add_library(${PROJECT_NAME}Timer_lib STATIC timer/view.cpp)
target_link_libraries(${PROJECT_NAME}Timer_lib PRIVATE ${Boost_LIBRARIES})
target_include_directories(${PROJECT_NAME}Timer_lib PRIVATE ${Boost_INCLUDE_DIRS})


add_library(${PROJECT_NAME}Match_lib STATIC match/view.cpp)
target_link_libraries(${PROJECT_NAME}Match_lib PRIVATE 
    HandlerRequest_lib 
    ${Boost_LIBRARIES}
)
target_include_directories(${PROJECT_NAME}Match_lib PRIVATE 
    ${Boost_INCLUDE_DIRS} 
    ${CMAKE_CURRENT_SOURCE_DIR} 
    ${CMAKE_CURRENT_SOURCE_DIR}/../
)

