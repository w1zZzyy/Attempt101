add_library(ControllerGame_lib STATIC game/view.hpp game/view.cpp)
target_link_libraries(ControllerGame_lib PUBLIC EventBus_lib LogicManager_lib)

add_library(ControllerUI_lib STATIC 
    ui/board/view.hpp 
    ui/board/view.cpp
)
target_link_libraries(ControllerUI_lib PUBLIC EventBus_lib UI_lib)

add_library(ControllerInput_lib STATIC input/view.hpp input/view.cpp)
target_link_libraries(ControllerInput_lib PUBLIC EventBus_lib SFML::Graphics)