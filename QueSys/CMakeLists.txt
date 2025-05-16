cmake_minimum_required(VERSION 3.23)

project(QueSysLib)

add_library(QueSys OBJECT)

target_sources(
    QueSys
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include/QueSys/queueing_system.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/queueing_system.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/QueSys/request.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/request.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/QueSys/event.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/event.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/requests_flow.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/requests_queue.hpp
)

target_include_directories(
    QueSys
    PRIVATE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/QueSys>
    PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)
