include_directories(${CMAKE_CURRENT_SOURCE_DIR})

add_subdirectory(core)
add_subdirectory(ui) 

target_link_libraries(Resources_lib PUBLIC SFML::Graphics Logic_lib)