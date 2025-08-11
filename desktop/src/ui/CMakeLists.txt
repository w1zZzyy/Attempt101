cmake_minimum_required(VERSION 3.20)
project(UI LANGUAGES CXX)

add_library(${PROJECT_NAME}_lib STATIC 
    piece/view.hpp piece/view.cpp 
    board/view.hpp board/view.cpp 
)
target_link_libraries(${PROJECT_NAME}_lib PUBLIC 
    SFML::Graphics
    Logic_lib
    ResourceTexture_lib
)
target_include_directories(${PROJECT_NAME}_lib PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/../
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../libs/
)