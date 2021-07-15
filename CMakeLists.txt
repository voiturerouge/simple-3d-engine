# CMake minimum version
cmake_minimum_required(VERSION 3.13...3.19 FATAL_ERROR)

# Project name
project(basic3dengine
    VERSION 0.0.1
    LANGUAGES CXX
)

# System information
message("-- CMAKE_SYSTEM_INFO_FILE: ${CMAKE_SYSTEM_INFO_FILE}")
message("-- CMAKE_SYSTEM_NAME:      ${CMAKE_SYSTEM_NAME}")
message("-- CMAKE_SYSTEM_PROCESSOR: ${CMAKE_SYSTEM_PROCESSOR}")
message("-- CMAKE_SYSTEM:           ${CMAKE_SYSTEM}")

# Packages
find_package(Qt5 5.7.0 REQUIRED
    COMPONENTS
        Core
        Gui
        3DCore
        #Qml
)

# Create target
add_executable(basic3dengine
    src/animation.cpp
    src/camera.cpp
    src/loader.cpp
    src/main.cpp
    src/mainwindow.cpp
    src/mesh.cpp
    src/renderer.cpp
)

# Set target properties
target_include_directories(basic3dengine
    PRIVATE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)
target_link_libraries(basic3dengine
    PUBLIC
        Qt5::Core
)


