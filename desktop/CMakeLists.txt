file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/assets DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
add_definitions(-DASSETS_PATH="${CMAKE_CURRENT_BINARY_DIR}/assets")

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src/)

include(FetchContent)

FetchContent_Declare(
    json
    GIT_REPOSITORY https://github.com/nlohmann/json
    GIT_TAG v3.11.3 
)
FetchContent_MakeAvailable(json)

set(SFML_BUILD_AUDIO OFF CACHE INTERNAL "")
set(SFML_BUILD_NETWORK OFF CACHE INTERNAL "")
set(SFML_BUILD_EXAMPLES OFF CACHE INTERNAL "")
set(SFML_BUILD_DOC OFF CACHE INTERNAL "")
set(SFML_USE_UDEV OFF CACHE INTERNAL "")

FetchContent_Declare( 
    sfml 
    GIT_REPOSITORY https://github.com/SFML/SFML 
    GIT_TAG master 
) 
FetchContent_MakeAvailable(sfml)


add_subdirectory(src/controllers)
add_subdirectory(src/ui)
add_subdirectory(src/resources)
add_subdirectory(src/scene)
add_subdirectory(src/event)

add_executable(${PROJECT_NAME}Desktop src/main.cpp src/application/view.hpp src/application/view.cpp)
target_link_libraries(${PROJECT_NAME}Desktop PRIVATE Logic_lib SceneManager_lib) 
