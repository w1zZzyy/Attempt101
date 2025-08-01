cmake_minimum_required(VERSION 3.14)
project(ChessUI LANGUAGES CXX) 

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(EXPORT_COMPILE_COMMANDS ON)

add_executable(${PROJECT_NAME} src/main.cpp)
target_link_libraries(${PROJECT_NAME} 
    sfml-graphics sfml-window sfml-system
)

