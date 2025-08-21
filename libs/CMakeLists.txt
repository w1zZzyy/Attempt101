set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -ffast-math -march=native -flto -DNDEBUG")
set(INCDIR ${CMAKE_CURRENT_SOURCE_DIR}/include)

add_subdirectory(src/logic)
add_subdirectory(src/engine)
add_subdirectory(src/manager)
add_subdirectory(tests)