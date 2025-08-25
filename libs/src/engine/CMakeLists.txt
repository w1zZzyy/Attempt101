set(ENGINE_INC ${INCDIR}/engine)
add_library(Engine_lib STATIC 
    ${ENGINE_INC}/eval.hpp 
    ${ENGINE_INC}/search.hpp 
    ${ENGINE_INC}/moveordering.hpp
    ${ENGINE_INC}/tt.hpp 
    eval.cpp
    search.cpp
    moveordering.cpp
    tt.cpp
)
target_include_directories(Engine_lib PRIVATE ${ENGINE_INC})
target_link_libraries(Engine_lib PUBLIC Logic_lib)