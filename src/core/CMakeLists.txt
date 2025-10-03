set(CMAKE_CXX_FLAGS_RELEASE "-O3 -ffast-math -march=native -flto -DNDEBUG")

add_subdirectory(logic)
add_subdirectory(engine)
add_subdirectory(tests)

target_include_directories(Engine_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(tests_exe PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
