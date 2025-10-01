add_library(GameState_lib STATIC 
    state/piece/idle.hpp state/piece/idle.cpp
    state/piece/in-progress.hpp state/piece/in-progress.cpp
    state/board/idle.hpp state/board/idle.cpp
    state/board/in-progress.hpp state/board/in-progress.cpp
    state/board/piece-selected.hpp state/board/piece-selected.cpp
)
target_link_libraries(GameState_lib PUBLIC Renderer_lib Logic_lib Shared_lib)

add_library(GameHandler_lib STATIC 
    handler/ui.hpp handler/ui.cpp 
)
target_link_libraries(GameHandler_lib PUBLIC Renderer_lib Shared_lib)