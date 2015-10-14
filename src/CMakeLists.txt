#include_directories("../include")
add_definitions(-fPIC -Wall -Werror -g -DDEBUG)
add_library(display OBJECT display.cpp)
add_library(geometry OBJECT geometry.cpp)
