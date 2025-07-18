cmake_minimum_required(VERSION 3.20)

project(Handler LANGUAGES CXX) 

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/ ${CMAKE_CURRENT_SOURCE_DIR}/../ ${CMAKE_CURRENT_SOURCE_DIR}/../../utils/)

add_library(${PROJECT_NAME}Response_lib STATIC response/view.cpp)
target_link_libraries(${PROJECT_NAME}Response_lib PUBLIC ${Boost_LIBRARIES})

add_library(${PROJECT_NAME}Request_lib STATIC request/view.cpp)
target_link_libraries(${PROJECT_NAME}Request_lib PRIVATE ${PROJECT_NAME}Response_lib)