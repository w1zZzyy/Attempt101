# GAME
add_library(ControllerGame_lib STATIC game/view.hpp game/view.cpp)
target_link_libraries(ControllerGame_lib PUBLIC EventBus_lib LogicManager_lib)

# UI
add_library(ControllerUI_lib STATIC 
    ui/board/view.hpp 
    ui/board/view.cpp
)
target_link_libraries(ControllerUI_lib PUBLIC EventBus_lib UI_lib)

# INPUT
add_library(ControllerInput_lib STATIC input/view.hpp input/view.cpp)
target_link_libraries(ControllerInput_lib PUBLIC EventBus_lib SFML::Graphics)

# HUMAN USER
add_library(ControllerHuman_lib STATIC user/human/view.hpp user/human/view.cpp)
target_link_libraries(ControllerHuman_lib PUBLIC EventBus_lib Logic_lib)

# AI USER
add_library(ControllerAI_lib STATIC user/ai/view.hpp user/ai/view.cpp)
target_link_libraries(ControllerAI_lib PUBLIC EventBus_lib Engine_lib)