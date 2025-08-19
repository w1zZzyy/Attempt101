add_library(ResourceTexture_lib STATIC 
    textures_manager/view.hpp 
    textures_manager/view.cpp
)
target_link_libraries(ResourceTexture_lib PUBLIC 
    SFML::Graphics
    Logic_lib    
)

add_library(ResourceConfig_lib STATIC 
    config_manager/view.hpp 
    config_manager/view.cpp 
)
target_link_libraries(ResourceConfig_lib PUBLIC 
    SFML::Graphics
    nlohmann_json::nlohmann_json 
    Logic_lib
)