set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -ffast-math -march=native -flto -DNDEBUG")

# ЛОГИКА

add_library(Logic_lib STATIC 
    include/logic/attack.hpp
    include/logic/defs.hpp
    include/logic/bitboard.hpp
    include/logic/move.hpp
    include/logic/movelist.hpp
	include/logic/position.hpp
    include/logic/zobrist.hpp
    include/logic/square.hpp 
    include/logic/storage.hpp
    src/logic/attack.cpp
    src/logic/defs.cpp
    src/logic/bitboard.cpp
    src/logic/move.cpp
    src/logic/movelist.cpp
	src/logic/position.cpp
    src/logic/zobrist.cpp
    src/logic/square.cpp 
    src/logic/storage.cpp
)
target_include_directories(Logic_lib PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include/logic)
target_include_directories(Logic_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
if (CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
	target_compile_options(Logic_lib PRIVATE -mbmi -mbmi2)
endif()


# ДВИЖОК

add_library(Engine_lib STATIC 
    include/engine/eval.hpp
    include/engine/search.hpp 
    include/engine/moveordering.hpp
    src/engine/eval.cpp
    src/engine/search.cpp
    src/engine/moveordering.cpp
)
target_include_directories(Engine_lib PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include/engine)
target_include_directories(Engine_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(Engine_lib PUBLIC Logic_lib)


# ТЕСТЫ

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/tests/tests_data DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

add_executable(tests_exe 
    tests/test_movegen.hpp 
    tests/test_movegen.cpp 
    tests/main.cpp
)
target_link_libraries(tests_exe PRIVATE Logic_lib)
target_compile_definitions(tests_exe PRIVATE 
    LOGIC_TESTS_DATA_PATH="${CMAKE_CURRENT_BINARY_DIR}/tests_data"
)



# УПРАВЛЕНИЕ ЛОГИКОЙ

add_library(LogicManager_lib STATIC 
    include/manager/logic.hpp 
    src/manager/logic.cpp
)
target_link_libraries(LogicManager_lib PUBLIC Logic_lib) 
target_include_directories(LogicManager_lib PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include/manager)


