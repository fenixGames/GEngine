/**
 * This file contains everything about the Game Engine..
 *
 * @author  Roberto Fernandez Cueto
 * @date    26.11.2015
 *
 * $Id$
 */

#ifndef _GENGINE_H
#define _GENGINE_H

namespace GEngine {
	class Display;
    class Camera;
    class StaticCamera;
    namespace Geometry {
        class Figure;
        class StaticFigure;
        class Face;
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
        class RegPolyhedron;
        class Prism;
        class Pyramid;
        class SphericalPlane;
        class Sphere;
        class EllipsoidalPlane;
        class Ellipsoid;
        class Cilinder;
        class Cone;
        class Toroid;
    };
    class Material;
    class Light;
}
#include "geometry.h"
#include "display.h"
#include "light.h"

#endif
