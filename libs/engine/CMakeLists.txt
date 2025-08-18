set(
    Header
    include/eval.hpp
    include/search.hpp
)

set(
    Source
    src/eval.cpp
    src/search.cpp
)

add_library(Engine_lib STATIC ${Header} ${Source})
target_link_libraries(Engine_lib PUBLIC Logic_lib)
target_include_directories(Engine_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include/)
