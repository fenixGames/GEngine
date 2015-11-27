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

# Find the dammed library.
find_library(LIBGL_PATH GL PATHS /usr/local/lib)
find_library(LIBGLUT_PATH glut PATHS /usr/local/lib)

message("LIBGL_PATH = ${LIBGL_PATH}")
message("LIBGLUT_PATH = ${LIBGLUT_PATH}")

set_property(TARGET GL PROPERTY IMPORTED_LOCATION ${LIBGL_PATH})
set_property(TARGET glut PROPERTY IMPORTED_LOCATION ${LIBGLUT_PATH})

# Setting the src subdirectory to be built before this one.
add_subdirectory("./src")

# Add the libgengine.so target to be linked with the src objects and linked with gl and glut
add_library(gengine SHARED 
		$<TARGET_OBJECTS:display> 
		$<TARGET_OBJECTS:geometry>
		$<TARGET_OBJECTS:matrix>
        $<TARGET_OBJECTS:camera>
        $<TARGET_OBJECTS:material>
		)
target_link_libraries(gengine GL glut)
set_target_properties(gengine PROPERTIES VERSION ${GEngine_VERSION_MAJOR}.${GEngine_VERSION_MINOR} 
    SOVERSION ${GEngine_VERSION_MAJOR}.${GEngine_VERSION_MINOR})

# Install target for the library.
install(TARGETS gengine LIBRARY DESTINATION /usr/local/lib)
install(DIRECTORY include/ DESTINATION /usr/local/include/gengine FILES_MATCHING PATTERN "*.h")

# Adding the deinstall target.
add_custom_target(deinstall COMMAND rm -rf /usr/local/lib/libgengine.so* /usr/local/include/gengine
    COMMENT "Deinstalling GEngine ... ")
