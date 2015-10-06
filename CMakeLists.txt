cmake_minimum_required(VERSION 2.6)

# Project name and version
project(GEngine CXX)
set(GEngine_VERSION_MAJOR   0)
set(GEngine_VERSION_MINOR   1)

# Setting the include and link directories - FreeBSD
include_directories(BEFORE "/usr/local/include" "${PROJECT_SOURCE_DIR}/include")
link_directories(/usr/local/lib)
add_library(GL SHARED IMPORTED)
add_library(glut SHARED IMPORTED)
set_property(TARGET GL PROPERTY IMPORTED_LOCATION /usr/local/lib/libGL.so)
set_property(TARGET glut PROPERTY IMPORTED_LOCATION /usr/local/lib/libglut.so)

add_subdirectory("./src")
