add_library(EventBus_lib STATIC bus/view.hpp bus/view.cpp models/event_type.hpp)
target_link_libraries(EventBus_lib PUBLIC SFML::Graphics Logic_lib)
target_include_directories(EventBus_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/models/)