file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/data DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

include(FetchContent)

FetchContent_Declare(
    gtest
    GIT_REPOSITORY https://github.com/google/googletest
    GIT_TAG v1.17.0
)

FetchContent_MakeAvailable(gtest)

enable_testing()

add_executable(tests_exe 
    src/main.cpp 
    src/test_node_counter.cpp 
    src/test_zobrist.cpp
)
target_link_libraries(tests_exe PRIVATE Logic_lib gtest_main)
target_compile_definitions(tests_exe PRIVATE 
    LOGIC_TESTS_DATA_PATH="${CMAKE_CURRENT_BINARY_DIR}/data"
)

include(GoogleTest)
gtest_discover_tests(tests_exe)
