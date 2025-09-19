set(ENGINE_INC ${INCDIR}/engine)
add_library(Engine_lib STATIC 
    ${ENGINE_INC}/eval.hpp 
    ${ENGINE_INC}/search.hpp 
    ${ENGINE_INC}/engine.hpp
    ${ENGINE_INC}/pick.hpp 
    ${ENGINE_INC}/tt.hpp
    ${ENGINE_INC}/timer.hpp
    eval.cpp
    search.cpp
    engine.cpp
    pick.cpp
    tt.cpp
    timer.cpp
)
target_include_directories(Engine_lib PRIVATE ${ENGINE_INC})
target_link_libraries(Engine_lib PUBLIC Logic_lib)