cmake_minimum_required(VERSION 3.0...3.27)

project(server)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_COMPILER g++)

set(CMAKE_BUILD_TYPE Debug)
set(RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

find_package(Boost REQUIRED)
include_directories(${Boost_INCLUDE_DIR})

set(DIR_SOURCES_COMMON "${PROJECT_SOURCE_DIR}/../common")
set(DIR_SOURCES "${PROJECT_SOURCE_DIR}")
file(GLOB SOURCES_COMMON "${DIR_SOURCES_COMMON}/*.cpp")
file(GLOB SOURCES "${DIR_SOURCES}/*.cpp")


include_directories(${DIR_SOURCES_COMMON})
include_directories(${DIR_SOURCES})

add_executable(${PROJECT_NAME} 
    ${SOURCES_COMMON}
    ${SOURCES}
    )

set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)

target_link_libraries(${PROJECT_NAME} ${Boost_LIBRARIES})
