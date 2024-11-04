cmake_minimum_required(VERSION 3.10)

project(${PROJECT_NAME})

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(PROJECT_NAME
    QueSysLib
    )

set(LIBRARY_NAME
    QueSys
    )

set(LIBRARY_HEADERS_DIR
    include/${LIBRARY_NAME}
    )

set(LIBRARY_HEADERS
    ${LIBRARY_HEADERS_DIR}/queueing_system.h
    ${LIBRARY_HEADERS_DIR}/request.h
    ${LIBRARY_HEADERS_DIR}/requests_flow.h
    ${LIBRARY_HEADERS_DIR}/requests_queue.h
    ${LIBRARY_HEADERS_DIR}/event.h
    )

set(LIBRARY_SOURCE_DIR
    src
    )

set(LIBRARY_SOURCE
    ${LIBRARY_SOURCE_DIR}/queueing_system.cpp
    ${LIBRARY_SOURCE_DIR}/request.cpp
    ${LIBRARY_SOURCE_DIR}/event.cpp
    )

add_library(${LIBRARY_NAME} STATIC
    ${LIBRARY_HEADERS}
    ${LIBRARY_SOURCE}
    )
target_include_directories(${LIBRARY_NAME} PRIVATE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/${LIBRARY_NAME}>
    $<INSTALL_INTERFACE:include/${LIBRARY_NAME}>
    )

target_include_directories(${LIBRARY_NAME} PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
    )
