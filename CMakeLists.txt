cmake_minimum_required(VERSION 3.0)

project(task)
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_COMPILER g++)
set(CMAKE_BINARY_DIR ${PROJECT_SOURCE_DIR}/bin)

#include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${Boost_INCLUDE_DIR})

set(SOURCES_COMMON "${PROJECT_SOURCE_DIR}/src/common")
set(SOURCES_CLIENT "${PROJECT_SOURCE_DIR}/src/client")
set(SOURCES_SERVER "${PROJECT_SOURCE_DIR}/src/server")

add_executable(client
    ${SOURCES_COMMON}
    ${SOURCES_CLIENT}
    )

add_executable(server
    ${SOURCES_COMMON}
    ${SOURCES_SERVER}
    )

target_link_libraries(client ${Boost_LIBRARIES})
target_link_libraries(server ${Boost_LIBRARIES})
