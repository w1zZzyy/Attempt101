cmake_minimum_required(VERSION 3.12)
project(libs LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -march=native -flto")

add_subdirectory(logic)
add_subdirectory(engine)

add_library(LogicManager_lib STATIC manager/view.hpp manager/view.cpp)
target_link_libraries(LogicManager_lib PUBLIC Logic_lib)
target_include_directories(LogicManager_lib PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/)
target_compile_features(LogicManager_lib PUBLIC cxx_std_23)

