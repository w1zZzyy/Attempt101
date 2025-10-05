cmake_minimum_required(VERSION 3.20)
project(Attempt LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_POLICY_VERSION_MINIMUM 3.5)

file(COPY ${CMAKE_SOURCE_DIR}/assets DESTINATION ${CMAKE_BINARY_DIR})
add_definitions(-DASSETS_PATH="${CMAKE_BINARY_DIR}/assets/")

include(FetchContent)

FetchContent_Declare(
    sfml 
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 3.0.0
)

FetchContent_MakeAvailable(sfml)

add_subdirectory(src)