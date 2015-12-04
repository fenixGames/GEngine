#include_directories("../include")

set( DISPLAY_OS )

if ( ${WITH_GLUT} )
	set( DISPLAY_OS "display-glut.cpp" )
elseif ( ${WITH_XORG} )
	set( DISPLAY_OS "display-xorg.cpp" )
elseif ( ${WITH_WAYLAND} )
	set( DISPLAY_OS "display-wayland.cpp" )
endif( ${WITH_GLUT} )

add_definitions(-fPIC -Wall -Werror -g -DDEBUG)
add_library(display OBJECT ${DISPLAY_OS} display.cpp)
add_library(matrix	OBJECT matrix.cpp vector.cpp)
add_library(geometry OBJECT geometry2D.cpp geometry3D.cpp)
add_library(camera  OBJECT  camera.cpp)
add_library(material OBJECT material.cpp)
add_library(world   OBJECT  world.cpp light.cpp)
