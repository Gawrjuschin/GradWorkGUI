cmake_minimum_required(VERSION 3.23)

project(QueSysLib)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(QueSys OBJECT)

target_sources(
    QueSys
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include/QueSys/queueing_system.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/queueing_system.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/QueSys/request.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/request.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/QueSys/event.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/event.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/QueSys/requests_flow.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/QueSys/requests_queue.h
)

target_include_directories(
    QueSys
    PRIVATE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/QueSys>
)

target_include_directories(
    QueSys
    PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)
