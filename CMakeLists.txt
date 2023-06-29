cmake_minimum_required(VERSION 3.0)

project(task)
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_COMPILER g++)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

add_executable(client
    client.cpp
    common.cpp
    client_functions.cpp
    )

add_executable(server
    server.cpp
    common.cpp
    server_functions.cpp
    )

target_link_libraries(client ${Boost_LIBRARIES})
target_link_libraries(server ${Boost_LIBRARIES})
