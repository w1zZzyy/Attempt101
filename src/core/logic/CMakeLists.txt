add_library(Logic_lib STATIC 
    attack.cpp attack.hpp  
    defs.cpp defs.hpp 
    bitboard.cpp bitboard.hpp
    move.cpp move.hpp 
    movelist.cpp movelist.hpp
	position.cpp position.hpp 
    zobrist.cpp zobrist.hpp
    square.cpp square.hpp 
    storage.cpp storage.hpp
)
if (CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
	target_compile_options(Logic_lib PRIVATE -mbmi -mbmi2)
endif()