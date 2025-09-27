add_library(Resources_lib STATIC
    resources/textures.hpp resources/textures.cpp 
    resources/colors.hpp resources/colors.cpp
    resources/text.hpp resources/text.cpp
)
target_link_libraries(Resources_lib PUBLIC SFML::Graphics Logic_lib)

add_library(UIUtils_lib STATIC 
    utils/quad.hpp utils/quad.cpp
)
target_link_libraries(UIUtils_lib PUBLIC SFML::Graphics)

add_library(Renderer_lib STATIC
    renderer/board.hpp renderer/board.cpp 
)
target_link_libraries(Renderer_lib PUBLIC Resources_lib UIUtils_lib)
target_include_directories(Renderer_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})