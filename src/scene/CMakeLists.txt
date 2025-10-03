add_library(Shared_lib STATIC 
    shared/bus.cpp
    shared/input.cpp
)
target_link_libraries(Shared_lib PUBLIC SFML::Graphics)

add_subdirectory(game)