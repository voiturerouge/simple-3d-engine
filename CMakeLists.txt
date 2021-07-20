# CMake minimum version
cmake_minimum_required(VERSION 3.13...3.19 FATAL_ERROR)

# Project name
project(simple3dengine
    VERSION 0.0.1
    LANGUAGES CXX
)

# System information
message("-- CMAKE_SYSTEM_INFO_FILE: ${CMAKE_SYSTEM_INFO_FILE}")
message("-- CMAKE_SYSTEM_NAME:      ${CMAKE_SYSTEM_NAME}")
message("-- CMAKE_SYSTEM_PROCESSOR: ${CMAKE_SYSTEM_PROCESSOR}")
message("-- CMAKE_SYSTEM:           ${CMAKE_SYSTEM}")

# C++
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Qt5
# Note: CMAKE_AUTOMOC and CMAKE_AUTOUIC will not process files unless there are in the same directory.
# Using qt5_wrap_cpp and qt5_wrap_ui instead.
set(CMAKE_AUTORCC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
find_package(Qt5 5.7.0 REQUIRED
    COMPONENTS
        Core
        Gui
        Widgets
)
qt5_wrap_cpp(MOC_SOURCES
    include/animation.h
    include/camera.h
    include/loader.h
    include/mainwindow.h
    include/mesh.h
    include/renderer.h
)
qt5_wrap_ui(UI_SOURCES
    data/forms/mainwindow.ui
)

# Create target
add_executable(simple3dengine
    src/animation.cpp
    src/camera.cpp
    src/loader.cpp
    src/main.cpp
    src/mainwindow.cpp
    src/mesh.cpp
    src/renderer.cpp
    ${MOC_SOURCES}
    ${UI_SOURCES}
)

# Set target properties
target_include_directories(simple3dengine
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)
target_link_libraries(simple3dengine
    PUBLIC
        Qt5::Core
        Qt5::Gui
        Qt5::Widgets
)