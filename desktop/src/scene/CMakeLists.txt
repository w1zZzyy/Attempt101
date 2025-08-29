add_library(SceneGame_lib STATIC game/view.hpp game/view.cpp)
target_link_libraries(SceneGame_lib PUBLIC 
    ResourceConfig_lib 
    ControllerGame_lib 
    ControllerUI_lib
    ControllerInput_lib 
    ControllerAI_lib
)
target_include_directories(SceneGame_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/)

add_library(SceneManager_lib STATIC manager/view.hpp manager/view.cpp)
target_link_libraries(SceneManager_lib PUBLIC SceneGame_lib)
