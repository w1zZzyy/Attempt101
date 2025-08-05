cmake_minimum_required(VERSION 3.20)
project(UI LANGUAGES CXX) 

# ENTITY
add_library(${PROJECT_NAME}Entity_lib STATIC entity/view.hpp entity/view.cpp)
target_link_libraries(${PROJECT_NAME}Entity_lib PUBLIC SFML::Graphics)
target_include_directories(${PROJECT_NAME}Entity_lib PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../libs
)

# BOARD 
add_library(${PROJECT_NAME}Board_lib STATIC board/view.hpp board/view.cpp)
target_link_libraries(${PROJECT_NAME}Board_lib PUBLIC ${PROJECT_NAME}Entity_lib)

# PIECE 
add_library(${PROJECT_NAME}Piece_lib STATIC piece/view.hpp piece/view.cpp)
target_link_libraries(${PROJECT_NAME}Piece_lib PUBLIC 
    ${PROJECT_NAME}Entity_lib
    ResourceTexture_lib
)

# SCENE 
add_library(${PROJECT_NAME}Scene_lib STATIC scene/view.hpp scene/view.cpp)
target_link_libraries(${PROJECT_NAME}Scene_lib PUBLIC 
    ${PROJECT_NAME}Board_lib
    ${PROJECT_NAME}Piece_lib
    Logic_lib
    Engine_lib 
    ResourceTexture_lib 
    ResourceConfig_lib 
)
target_include_directories(${PROJECT_NAME}Scene_lib PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}/
    ${CMAKE_CURRENT_SOURCE_DIR}/../
)