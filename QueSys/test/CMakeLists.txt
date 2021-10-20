cmake_minimum_required(VERSION 3.5)

set(TEST_PROJECT_NAME
	GradWork_Demonstration)
set(TESTS_SOURCE
	demonstration.cpp)

project(${TEST_PROJECT_NAME})
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(gradwork_nogui ${TESTS_SOURCE})

target_link_libraries(${TEST_PROJECT_NAME} ${LIBRARY_NAME})
