cmake_minimum_required(VERSION 3.20)
project(ChessUI LANGUAGES CXX) 

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_subdirectory(SFML)
add_subdirectory(json)

# BOARD 
add_library(Board_lib STATIC 
    src/ui/board/view.hpp 
    src/ui/board/view.cpp
)
target_link_libraries(Board_lib PUBLIC SFML::Graphics)
target_include_directories(Board_lib PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}/../libs
)

# SCENE
add_library(Scene_lib STATIC 
    src/ui/scene/view.hpp 
    src/ui/scene/view.cpp
)
target_link_libraries(Scene_lib PUBLIC 
    Board_lib
    SFML::Graphics SFML::System SFML::Window
    nlohmann_json::nlohmann_json    
)

# WINDOW
add_library(Window_lib STATIC 
    src/ui/window/view.hpp 
    src/ui/window/view.cpp
)
target_link_libraries(Window_lib PUBLIC Scene_lib)
target_include_directories(Board_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src/ui)

# MAIN
add_executable(${PROJECT_NAME} src/main.cpp)
target_link_libraries(${PROJECT_NAME} PRIVATE Window_lib)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/assets DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
add_definitions(-DASSETS_PATH="${CMAKE_CURRENT_BINARY_DIR}/assets")
