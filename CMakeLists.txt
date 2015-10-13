cmake_minimum_required(VERSION 2.6)

# Project name and version
project(GEngine CXX)
set(GEngine_VERSION_MAJOR   0)
set(GEngine_VERSION_MINOR   1)

# Setting the include and link directories - FreeBSD
include_directories(BEFORE "/usr/local/include" "${PROJECT_SOURCE_DIR}/include")
link_directories(/usr/local/lib)

# Adding the libraries for this project (GL and glut)
add_library(GL SHARED IMPORTED)
add_library(glut SHARED IMPORTED)
set_property(TARGET GL PROPERTY IMPORTED_LOCATION /usr/local/lib/libGL.so)
set_property(TARGET glut PROPERTY IMPORTED_LOCATION /usr/local/lib/libglut.so)

# Setting the src subdirectory to be built before this one.
add_subdirectory("./src")

# Add the libgengine.so target to be linked with the src objects and linked with gl and glut
add_library(gengine SHARED $<TARGET_OBJECTS:display> $<TARGET_OBJECTS:geometry>)
target_link_libraries(gengine GL glut)
