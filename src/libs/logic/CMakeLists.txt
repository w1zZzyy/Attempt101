cmake_minimum_required(VERSION 3.20 FATAL_ERROR)

project(Logic LANGUAGES CXX) 

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set (
    Header 
    src/attack.hpp
    src/defs.hpp
    src/bitboard.hpp
    src/move.hpp
    src/movelist.hpp
	src/position.hpp
    src/zobrist.hpp
    src/square.hpp 
    src/storage.hpp
)

set(
    Source 
    src/attack.cpp
    src/defs.cpp
    src/bitboard.cpp
    src/move.cpp
    src/movelist.cpp
	src/position.cpp
    src/zobrist.cpp
    src/square.cpp 
    src/storage.cpp
)

add_library(${PROJECT_NAME}_lib STATIC ${Header} ${Source})

if (CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
	target_compile_options(${PROJECT_NAME}_lib PUBLIC -mbmi2 -mbmi)
endif()

set(
    Tests 
    tests/test_movegen.hpp 
    tests/test_movegen.cpp
    tests/main.cpp
)

add_executable(tests_exe ${Tests})
target_link_libraries(tests_exe PRIVATE ${PROJECT_NAME}_lib)
target_include_directories(tests_exe PRIVATE src)