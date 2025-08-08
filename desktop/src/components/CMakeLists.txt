cmake_minimum_required(VERSION 3.20)
project(Component LANGUAGES CXX)

add_library(${PROJECT_NAME}Game_lib STATIC game/view.hpp game/view.cpp)
target_link_libraries(${PROJECT_NAME}Game_lib PUBLIC EventBus_lib LogicManager_lib)

add_library(${PROJECT_NAME}UI_lib STATIC 
    ui/piece/view.hpp ui/piece/view.cpp 
    ui/board/view.hpp ui/board/view.cpp 
)
target_link_libraries(${PROJECT_NAME}UI_lib PUBLIC 
    SFML::Graphics
    Logic_lib
    EventBus_lib 
    ResourceTexture_lib
)