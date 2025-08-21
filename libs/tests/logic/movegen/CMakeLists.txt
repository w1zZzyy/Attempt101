file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/data DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

add_library(TestMovegen STATIC  
    test.hpp 
    test.cpp
)
target_link_libraries(TestMovegen PUBLIC Logic_lib)
target_compile_definitions(TestMovegen PUBLIC 
    LOGIC_TESTS_DATA_PATH="${CMAKE_CURRENT_BINARY_DIR}/data"
)