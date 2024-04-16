cmake_minimum_required(VERSION 2.8)

include($ENV{DEVKIT_LOCATION}/cmake/pluginEntry.cmake)

set(PROJECT_NAME findCollision)

project(${PROJECT_NAME})

set(SOURCE_FILES
    findCollision.h
    findCollision.cpp
)

set(LIBRARIES
    OpenMaya
    Foundation
)

build_plugin()
