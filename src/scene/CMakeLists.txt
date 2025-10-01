add_library(Shared_lib STATIC 
    shared/bus.hpp shared/bus.cpp
    shared/input.hpp shared/input.cpp
)
target_link_libraries(Shared_lib PUBLIC SFML::Graphics)
target_include_directories(Shared_lib PUBLIC ${SRC_DIR})

add_subdirectory(game)