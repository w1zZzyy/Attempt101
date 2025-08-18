set (
    Header 
    include/attack.hpp
    include/defs.hpp
    include/bitboard.hpp
    include/move.hpp
    include/movelist.hpp
	include/position.hpp
    include/zobrist.hpp
    include/square.hpp 
    include/storage.hpp
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

add_library(Logic_lib STATIC ${Header} ${Source})
target_include_directories(Logic_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include/)

if (CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
	target_compile_options(Logic_lib PUBLIC -mbmi2 -mbmi)
endif()

set(
    Tests 
    tests/test_movegen.hpp 
    tests/test_movegen.cpp
    tests/main.cpp
)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/tests/tests_data DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
add_definitions(-DLOGIC_TESTS_DATA_PATH="${CMAKE_CURRENT_BINARY_DIR}/tests_data")

add_executable(tests_exe ${Tests})
target_link_libraries(tests_exe PRIVATE Logic_lib)
target_include_directories(tests_exe PRIVATE src)