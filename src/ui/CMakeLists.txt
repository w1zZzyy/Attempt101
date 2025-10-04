add_library(UI_lib STATIC 
    resources/textures.cpp 
    resources/colors.cpp
    resources/text.cpp
    renderer/board.cpp
    renderer/pieces.cpp 
    renderer/promotion.cpp
    utils/quad.cpp
    model/options.cpp 
)
target_link_libraries(UI_lib PUBLIC SFML::Graphics Logic_lib)