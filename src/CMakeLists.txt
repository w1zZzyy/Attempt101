include_directories(${CMAKE_CURRENT_SOURCE_DIR})

add_subdirectory(core)
add_subdirectory(ui) 
add_subdirectory(scene)
add_subdirectory(application)

add_executable(main main.cpp)
target_link_libraries(main PRIVATE Application_lib)