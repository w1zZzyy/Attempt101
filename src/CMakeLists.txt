set(SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR})

add_subdirectory(core)
add_subdirectory(ui) 
add_subdirectory(scene/game)

add_executable(main main.cpp)
target_link_libraries(main PRIVATE Renderer_lib GameState_lib)