cmake_minimum_required(VERSION 3.0)

project(task)
set(CMAKE_BUILD_TYPE Release)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_COMPILER g++)
set(CMAKE_BINARY_DIR ${PROJECT_SOURCE_DIR}/bin)

set(BOOST_ROOT /usr/include)
include_directories(${Boost_INCLUDE_DIR})

set(DIR_SOURCES_COMMON "${PROJECT_SOURCE_DIR}/src/common")
set(DIR_SOURCES_CLIENT "${PROJECT_SOURCE_DIR}/src/client")
set(DIR_SOURCES_SERVER "${PROJECT_SOURCE_DIR}/src/server")
file(GLOB SOURCES_COMMON "${DIR_SOURCES_COMMON}/*.cpp" "${DIR_SOURCES_COMMON}/*.h")
file(GLOB SOURCES_CLIENT "${DIR_SOURCES_CLIENT}/*.cpp" "${DIR_SOURCES_CLIENT}/*.h")
file(GLOB SOURCES_SERVER "${DIR_SOURCES_SERVER}/*.cpp" "${DIR_SOURCES_SERVER}/*.h")

include_directories(${DIR_SOURCES_COMMON})
include_directories(${DIR_SOURCES_CLIENT})
include_directories(${DIR_SOURCES_SERVER})

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
