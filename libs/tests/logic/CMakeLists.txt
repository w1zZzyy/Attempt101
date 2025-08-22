add_subdirectory(movegen)

add_executable(tests_exe main.cpp)
target_link_libraries(tests_exe PRIVATE TestMovegen)