cmake_minimum_required(VERSION 3.20)
project(ChessDesktop LANGUAGES CXX) 

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/assets DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
add_definitions(-DASSETS_PATH="${CMAKE_CURRENT_BINARY_DIR}/assets")

add_library(Application STATIC src/application/view.hpp src/application/view.cpp)
target_link_libraries(Application PUBLIC UIScene_lib)
target_include_directories(Application PUBLIC src/ui)

add_subdirectory(SFML)
add_subdirectory(json)
add_subdirectory(src/ui)
add_subdirectory(src/resources)

add_executable(${PROJECT_NAME} src/main.cpp)
target_link_libraries(${PROJECT_NAME} PRIVATE Application)
