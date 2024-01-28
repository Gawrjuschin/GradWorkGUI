cmake_minimum_required(VERSION 3.5)

set(PROJECT_NAME GradWorkGUI)
set(APP_NAME gradwork_gui)
project(${PROJECT_NAME} VERSION 1.0 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY  ${CMAKE_BINARY_DIR}/lib)

find_package(QT NAMES Qt5 COMPONENTS Core Widgets Charts REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core Widgets Charts REQUIRED)

set(PROJECT_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/backend_object.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/backend_object.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/graphs_data.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/graphs_data.cpp
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
    ${CMAKE_CURRENT_SOURCE_DIR}/src/progress.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/results_widget.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/results_widget.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/status_bar.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/status_bar.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/synchronizer.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/synchronizer.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/table_data.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/table_data.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/table_widget.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/table_widget.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/worker_object.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/worker_table.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/resources.qrc
    ${CMAKE_CURRENT_SOURCE_DIR}/src/backendobject.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/backendobject.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/threadscontrol.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/threadscontrol.cpp
    )

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(${APP_NAME}
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )

# Define target properties for Android with Qt 6 as:
#    set_property(TARGET untitled APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(${APP_NAME} SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(${APP_NAME}
            ${PROJECT_SOURCES}
        )
    endif()
endif()

add_subdirectory(QueSys)

target_link_libraries(${APP_NAME} PRIVATE Qt${QT_VERSION_MAJOR}::Core
                                  PRIVATE Qt${QT_VERSION_MAJOR}::Widgets
                                  PRIVATE Qt${QT_VERSION_MAJOR}::Charts
                                  PRIVATE QueSys)

set_target_properties(${APP_NAME} PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.${APP_NAME}.edu
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(${APP_NAME})
endif()
