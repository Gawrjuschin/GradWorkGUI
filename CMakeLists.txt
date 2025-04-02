cmake_minimum_required(VERSION 3.23)

project(
    GradWorkGUI
    VERSION 1.0
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# TODO: Qt5 compatibility
find_package(Qt6 COMPONENTS Core Widgets Charts REQUIRED)
qt_standard_project_setup()

add_subdirectory(QueSys)

set(GRADWORKGUI_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/points_data.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/graphs_switch.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/graphs_switch.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/graphs_view.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/graphs_view.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/graphs_widget.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/graphs_widget.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/input_data.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/input_widget.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/input_widget.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/loading_widget.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/loading_widget.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/main_window.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/main_window.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/models.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/models.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/progress.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/results_widget.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/results_widget.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/status_bar.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/status_bar.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/synchronizer.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/synchronizer.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/table_data.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/table_widget.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/table_widget.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/backendobjectworker.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/backendobject.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/threadscontrol.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/threadscontrol.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/simulationworker.h
)

qt_add_resources(
    GRADWORKGUI_SOURCES
    src/resources.qrc
)

qt_add_executable(gradwork_gui)

target_sources(
    gradwork_gui
    PRIVATE
    ${GRADWORKGUI_SOURCES}
)

target_link_libraries(
    gradwork_gui
    PRIVATE
    Qt6::Core Qt6::Widgets Qt6::Charts
    QueSys
)

set_target_properties(
    gradwork_gui
    PROPERTIES
    WIN32_EXECUTABLE ON
    MACOSX_BUNDLE ON
)
