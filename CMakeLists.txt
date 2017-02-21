cmake_minimum_required(VERSION 2.8.7)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
project(YDCLIENT)

set(YDCLIENT_VERSION_MAJOR 0)
set(YDCLIENT_VERSION_MINOR 1)
set(YDCLIENT_VERSION_PATCH 0)
set(YDCLIENT_VERSION ${YDCLIENT_VERSION_MAJOR}.${YDCLIENT_VERSION_MINOR}.${YDCLIENT_VERSION_PATCH})
set(YDCLIENT_VERSION_STRING "v${YDCLIENT_VERSION}")

option(BUILD_TESTS "Build tests" ON)

find_package(Boost COMPONENTS filesystem system)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

file(GLOB ${PROJECT_NAME}_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/sources/*) 

add_library(${PROJECT_NAME} ${${PROJECT_NAME}_SOURCES})

target_link_libraries(${PROJECT_NAME} Boost::filesystem Boost::system)

if(BUILD_TESTS)
	enable_testing()
    include_directories(${CMAKE_CURRENT_SOURCE_DIR}/tests/)
	file(GLOB TESTS_${PROJECT_NAME}_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/tests/*/*.cpp)
	add_executable(check ${TESTS_${PROJECT_NAME}_SOURCES} ${CMAKE_CURRENT_SOURCE_DIR}/tests/main.cpp)
	target_link_libraries(check ${PROJECT_NAME} ${DEPENDS_LIBRARIES})
	add_test(NAME check COMMAND check "-s" "-r" "compact" "--use-colour" "yes")	
endif()
