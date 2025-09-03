add_library(UI_lib STATIC 
    piece/view.hpp piece/view.cpp 
    piece/manager.hpp piece/manager.cpp 
    piece/selected.hpp piece/selected.cpp
    board/view.hpp board/view.cpp 
    board/manager.hpp board/manager.cpp
)
target_link_libraries(UI_lib PUBLIC 
    SFML::Graphics
    Logic_lib 
    EventBus_lib
    ResourceTexture_lib
)