#ifndef _GENGINE_H
#define _GENGINE_H

namespace GEngine {
	class Display;
    struct camera;
    namespace Geometry {
        class Figure;
        class StaticFigure;
        class Point;
        class Arc;
        class Sector;
        class Circle;
        class Segment;
        class Polygon;
        class EllArc;
        class Ellipse;
        class RegPol;
        class Rectangle;
        class Polyhedra;
    };

}
#include "geometry.h"
#include "display.h"

#endif
