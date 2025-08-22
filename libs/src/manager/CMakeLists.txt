set(MANAGER_INC ${INCDIR}/manager)
add_library(LogicManager_lib STATIC 
    ${MANAGER_INC}/logic.hpp 
    logic.cpp
)
target_link_libraries(LogicManager_lib PUBLIC Logic_lib) 
target_include_directories(LogicManager_lib PRIVATE ${MANAGER_INC})