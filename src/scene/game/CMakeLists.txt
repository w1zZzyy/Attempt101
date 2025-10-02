add_library(GameScene_lib STATIC 
    state/piece/idle.cpp
    state/piece/in-progress.cpp
    state/board/idle.cpp
    state/board/in-progress.cpp
    state/board/piece-selected.cpp
    handler/game.cpp 
    handler/ui.cpp
    scene.cpp 
)
target_link_libraries(GameScene_lib PUBLIC UI_lib Logic_lib Shared_lib)