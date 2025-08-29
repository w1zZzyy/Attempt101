add_library(EventBus_lib STATIC 
    bus/view.hpp bus/view.cpp 
    models/error_event.hpp 
    models/event_type.hpp 
    models/mouse_event.hpp 
    models/position_event.hpp 
)
target_link_libraries(EventBus_lib PUBLIC SFML::Graphics Logic_lib)
target_include_directories(EventBus_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/models/)