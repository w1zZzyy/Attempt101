add_library(Resources_lib STATIC
    resources/textures.hpp resources/textures.cpp 
    resources/colors.hpp resources/colors.cpp
    resources/text.hpp resources/text.cpp
)
target_link_libraries(Resources_lib PUBLIC SFML::Graphics Logic_lib)
target_include_directories(Resources_lib PUBLIC ${SRC_DIR})

add_library(UIUtils_lib STATIC 
    utils/quad.hpp utils/quad.cpp
    utils/cellsize.hpp utils/cellsize.cpp
)
target_link_libraries(UIUtils_lib PUBLIC SFML::Graphics)

add_library(Renderer_lib STATIC
    renderer/board.hpp renderer/board.cpp 
    renderer/pieces.hpp renderer/pieces.cpp 
)
target_link_libraries(Renderer_lib PUBLIC Resources_lib UIUtils_lib)