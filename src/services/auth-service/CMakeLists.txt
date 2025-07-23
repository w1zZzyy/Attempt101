cmake_minimum_required(VERSION 3.12)
project(auth LANGUAGES CXX)

find_package(userver COMPONENTS core postgresql QUIET)
if(NOT userver_FOUND)  
	set(USERVER_FEATURE_POSTGRESQL ON CACHE BOOL "" FORCE)

	# Compatibility mode: some systems don't support these features
	set(USERVER_FEATURE_CRYPTOPP_BLAKE2 OFF CACHE BOOL "" FORCE)
	set(USERVER_FEATURE_GRPC_CHANNELZ OFF CACHE BOOL "" FORCE)
	set(USERVER_FEATURE_REDIS_HI_MALLOC ON CACHE BOOL "" FORCE)

	if (EXISTS third_party/userver)
		message(STATUS "Using userver framework from third_party/userver")
		add_subdirectory(third_party/userver)
	else()
		message(FATAL_ERROR "Either install the userver or provide a path to it")
	endif()
endif()

userver_setup_environment()


add_library(${PROJECT_NAME}_lib STATIC 
    src/handlers/register/view.cpp src/handlers/register/view.hpp
    src/handlers/login/view.cpp src/handlers/login/view.hpp 
    src/models/user.hpp src/models/user.cpp
)
target_link_libraries(${PROJECT_NAME}_lib PUBLIC userver::postgresql)


add_executable(${PROJECT_NAME} src/main.cpp)
target_link_libraries(${PROJECT_NAME} PRIVATE ${PROJECT_NAME}_lib)


userver_testsuite_add_simple()

include(GNUInstallDirs)

if(DEFINED ENV{PREFIX})
  message(STATUS "Set install prefix: $ENV{PREFIX}")
  file(TO_CMAKE_PATH "$ENV{PREFIX}" PREFIX_PATH)
  set(CMAKE_INSTALL_PREFIX ${PREFIX_PATH})
endif()

file(GLOB CONFIGS_FILES ${CMAKE_CURRENT_SOURCE_DIR}/configs/*.yaml ${CMAKE_CURRENT_SOURCE_DIR}/configs/*.json)

install(TARGETS ${PROJECT_NAME} DESTINATION ${CMAKE_INSTALL_BINDIR} COMPONENT ${PROJECT_NAME})
install(FILES ${CONFIGS_FILES} DESTINATION ${CMAKE_INSTALL_SYSCONFDIR}/${PROJECT_NAME} COMPONENT ${PROJECT_NAME})

