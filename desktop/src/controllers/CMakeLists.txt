# GAME
add_library(ControllerGame_lib STATIC 
    game/view.hpp game/view.cpp 
    game/logic.hpp game/logic.cpp
)
target_link_libraries(ControllerGame_lib PUBLIC EventBus_lib Logic_lib)

# UI
add_library(ControllerUI_lib STATIC 
    ui/board/view.hpp 
    ui/board/view.cpp 
)
target_link_libraries(ControllerUI_lib PUBLIC EventBus_lib UI_lib)

# INPUT
add_library(ControllerInput_lib STATIC input/view.hpp input/view.cpp)
target_link_libraries(ControllerInput_lib PUBLIC EventBus_lib SFML::Graphics)

# AI USER
add_library(ControllerAI_lib STATIC ai/view.hpp ai/view.cpp)
target_link_libraries(ControllerAI_lib PUBLIC EventBus_lib Engine_lib)