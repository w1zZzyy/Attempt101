set(LOGIC_INC ${INCDIR}/logic)
add_library(Logic_lib STATIC 
    ${LOGIC_INC}/attack.hpp
    ${LOGIC_INC}/defs.hpp
    ${LOGIC_INC}/bitboard.hpp
    ${LOGIC_INC}/move.hpp
    ${LOGIC_INC}/movelist.hpp
    ${LOGIC_INC}/position.hpp
    ${LOGIC_INC}/zobrist.hpp
    ${LOGIC_INC}/square.hpp 
    ${LOGIC_INC}/storage.hpp
    attack.cpp
    defs.cpp
    bitboard.cpp
    move.cpp
    movelist.cpp
	position.cpp
    zobrist.cpp
    square.cpp 
    storage.cpp
)
target_include_directories(Logic_lib PRIVATE ${LOGIC_INC})
target_include_directories(Logic_lib PUBLIC ${INCDIR})
if (CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
	target_compile_options(Logic_lib PRIVATE -mbmi -mbmi2)
endif()