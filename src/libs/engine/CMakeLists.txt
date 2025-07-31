cmake_minimum_required(VERSION 3.20)

project(Engine LANGUAGES CXX) 

set(
    Header
    src/eval.hpp
    src/search.hpp
)

set(
    Source
    src/eval.cpp
    src/search.cpp
)

add_library(${PROJECT_NAME}_lib STATIC ${Header} ${Source})
target_link_libraries(${PROJECT_NAME}_lib PUBLIC Logic_lib)
target_include_directories(${PROJECT_NAME}_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../logic/src/)

add_executable(${PROJECT_NAME}_exe main.cpp)
target_link_libraries(${PROJECT_NAME}_exe PRIVATE ${PROJECT_NAME}_lib)
target_include_directories(${PROJECT_NAME}_exe PRIVATE src)