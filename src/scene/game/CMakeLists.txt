add_library(GameState_lib STATIC 
    state/piece/idle.hpp state/piece/idle.cpp
)
target_link_libraries(GameState_lib PUBLIC Renderer_lib Logic_lib)