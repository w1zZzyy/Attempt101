add_library(UI_lib STATIC 
    piece/view.hpp piece/view.cpp 
    board/view.hpp board/view.cpp 
)
target_link_libraries(UI_lib PUBLIC 
    SFML::Graphics
    Logic_lib 
    EventBus_lib
    ResourceTexture_lib
)