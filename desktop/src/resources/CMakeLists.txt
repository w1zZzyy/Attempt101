cmake_minimum_required(VERSION 3.20)
project(Resource LANGUAGES CXX) 

add_library(${PROJECT_NAME}Texture_lib STATIC 
    textures_manager/view.hpp 
    textures_manager/view.cpp
)
target_link_libraries(${PROJECT_NAME}Texture_lib PUBLIC SFML::Graphics)
target_include_directories(${PROJECT_NAME}Texture_lib PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../libs
)

add_library(${PROJECT_NAME}Config_lib STATIC 
    config_manager/view.hpp 
    config_manager/view.cpp
)
target_link_libraries(${PROJECT_NAME}Config_lib PUBLIC 
    SFML::Graphics
    nlohmann_json::nlohmann_json
)