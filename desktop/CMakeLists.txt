file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/assets DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
add_definitions(-DASSETS_PATH="${CMAKE_CURRENT_BINARY_DIR}/assets")

add_subdirectory(SFML)
add_subdirectory(json)
add_subdirectory(src/controllers)
add_subdirectory(src/ui)
add_subdirectory(src/resources)
add_subdirectory(src/scene)
add_subdirectory(src/event)

add_executable(${PROJECT_NAME} src/main.cpp src/application/view.hpp src/application/view.cpp)
target_link_libraries(${PROJECT_NAME} PRIVATE SceneManager_lib Logic_lib) 
