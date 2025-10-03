add_library(Engine_lib STATIC 
    eval.cpp eval.hpp 
    search.cpp search.hpp 
    pick.cpp pick.hpp 
    tt.cpp tt.hpp
    timer.cpp timer.hpp
)
target_link_libraries(Engine_lib PUBLIC Logic_lib)