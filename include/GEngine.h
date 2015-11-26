/**
 * This file contains everything about the Game Engine..
 *
 * @author  Roberto Fernandez Cueto
 * @date    26.11.2015
 *
 * $Id: $
 */

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
		class Mesh;
        class Polyhedron;
    };

}
#include "geometry.h"
#include "display.h"

#endif
