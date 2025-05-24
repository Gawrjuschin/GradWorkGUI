cmake_minimum_required(VERSION 3.23)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(
    GradWorkGUI
    VERSION 2.0
    LANGUAGES CXX
)

option(GRADWORKGUI_ENABLE_DOCS "Build doxygen docs" OFF)

# TODO: Qt5 compatibility
find_package(Qt6 COMPONENTS Core Widgets Charts REQUIRED)
qt_standard_project_setup()


if(GRADWORKGUI_ENABLE_DOCS)
    find_package(Doxygen REQUIRED)

    set(DOXYGEN_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/docs")
    set(DOXYGEN_GENERATE_HTML YES)
    set(DOXYGEN_GENERATE_MAN YES)
    set(DOXYGEN_MARKDOWN_SUPPORT YES)
    set(DOXYGEN_AUTOLINK_SUPPORT YES)
    set(DOXYGEN_HAVE_DOT NO)
    set(DOXYGEN_COLLABORATION_GRAPH YES)
    set(DOXYGEN_CLASS_GRAPH YES)
    set(DOXYGEN_UML_LOOK YES)
    set(DOXYGEN_DOT_UML_DETAILS YES)
    set(DOXYGEN_DOT_WRAP_THRESHOLD 100)
    set(DOXYGEN_CALL_GRAPH YES)
    set(DOXYGEN_QUIET YES)
    set(DOXYGEN_EXCLUDE_PATTERNS "moc_*")

    doxygen_add_docs(
        cmake_packaging_docs
        "${CMAKE_CURRENT_LIST_DIR}"
        ALL
        COMMENT "generate docs for gradwork_gui"
    )

endif()

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
    ${CMAKE_CURRENT_SOURCE_DIR}/src/progress.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/results_widget.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/results_widget.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/status_bar.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/status_bar.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/table_data.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/table_widget.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/table_widget.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/simulation_interface.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/simulation_control.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/quesys_simulation.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/threads_control.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/events_model.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/events_model.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/requests_model.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/requests_model.cpp
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
