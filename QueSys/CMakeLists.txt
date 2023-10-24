cmake_minimum_required(VERSION 3.10)

set(PROJECT_NAME
    QueSysLib
    )

set(LIBRARY_NAME
    QueSys
    )

set(EXAMPLE_NAME
    gradwork_nogui
    )

set(LIBRARY_HEADERS_DIR
    include/${LIBRARY_NAME}
    )

set(LIBRARY_HEADERS
    ${LIBRARY_HEADERS_DIR}/queueing_system.h
    ${LIBRARY_HEADERS_DIR}/request.h
    ${LIBRARY_HEADERS_DIR}/req_flow.h
    ${LIBRARY_HEADERS_DIR}/req_queue.h
    ${LIBRARY_HEADERS_DIR}/event.h
    ${LIBRARY_HEADERS_DIR}/system_parameters_types.h
    )

set(LIBRARY_SOURCE_DIR
    src
    )

set(LIBRARY_SOURCE
    ${LIBRARY_SOURCE_DIR}/queueing_system.cpp
    ${LIBRARY_SOURCE_DIR}/request.cpp
    ${LIBRARY_SOURCE_DIR}/event.cpp
    )

project(${PROJECT_NAME})
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

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

add_executable(${EXAMPLE_NAME} test/demonstration.cpp)

target_link_libraries(${EXAMPLE_NAME} ${LIBRARY_NAME})
