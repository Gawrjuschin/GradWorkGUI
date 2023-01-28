cmake_minimum_required(VERSION 3.5)

set(PROJECT_NAME GradWorkGUI)
set(APP_NAME gradwork_gui)
project(${PROJECT_NAME} VERSION 1.0 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY  ${CMAKE_BINARY_DIR}/lib)

find_package(QT NAMES Qt6 Qt5 COMPONENTS Core Widgets Charts REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core Widgets Charts REQUIRED)

set(PROJECT_SOURCES src/main.cpp
    src/backend_object.h
    src/backend_object.cpp
    src/graphs_data.h
    src/graphs_data.cpp
    src/graphs_switch.cpp
    src/graphs_switch.h
    src/graphs_view.cpp
    src/graphs_view.h
    src/graphs_widget.h
    src/graphs_widget.cpp
    src/input_widget.h
    src/input_widget.cpp
    src/loading_widget.h
    src/loading_widget.cpp
    src/main_window.h
    src/main_window.cpp
    src/models.h
    src/models.cpp
    src/progress.h
    src/progress.cpp
    src/results_widget.h
    src/results_widget.cpp
    src/status_bar.h
    src/status_bar.cpp
    src/synchronizer.h
    src/synchronizer.cpp
    src/system_data.h
    src/table_data.h
    src/table_data.cpp
    src/table_widget.h
    src/table_widget.cpp
    src/worker_object.h
    src/worker_object.cpp
    src/worker_table.h
    src/worker_table.cpp
    src/resources.qrc
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
