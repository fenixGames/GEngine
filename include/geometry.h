/**
 * This file contains everything about geometrical classes to be printed on the screen.
 *
 * @author  Roberto Fernandez Cueto
 * @date    12.10.2015
 */

#ifndef _GEOMETRY_H
#define _GEOMETRY_H

#include <list>
#include <GL/gl.h>

#define Point2DList std::list<Point2D *>
#define PI  3.141592653589793

/**
 * The abstract class for the printable objects.
 */
class Figure {
    protected:
        bool solid;
    public:
        Figure();
        virtual void print() = 0;
        void setSolid();
};

/**
 * The class used to print a 2D point.
 */
class Point2D : public Figure {
    friend class Arc2D;
    friend class Circunference2D;
    friend class Segment2D;
    friend class Polygon2D;
    protected:
        GLint   x, /* The horizontal component of the point. */
                y; /* The vertical component of the point. */
    public:
        Point2D(GLint xx = 0, GLint yy = 0);
        Point2D operator = (Point2D);
        static float distance(Point2D p1, Point2D p2);
        virtual void print();
};

/**
 * The class used to print a 3D point.
 */
class Point3D : public Point2D {
    protected:
        GLint z; /* The depth component of the point. */
    public:
        Point3D(GLint xx, GLint yy, GLint zz);
        void print();
};

/**
 * The class used to print an arc.
 */
class Arc2D : public Figure {
    protected:
        Point2D center; /* The point associated to the center of the arc. */
        Point2D start;  /* The starting point of the arc. */
        GLfloat angle;  /* The angle associated to the arc. */

    public:
        Arc2D(Point2D c, Point2D s, GLfloat a = 2 * PI);
        Point2D * getEnd();
        virtual void print();
};

/**
 * The class used to print a circular sector.
 */
class Sector2D : public Arc2D {
    public:
        void print();
};

/**
 * The class to print a circunference.
 */
class Circunference2D : public Arc2D {
    public:
        Circunference2D(Point2D c, GLint radius);
};

/**
 * The class used to print a segment.
 */
class Segment2D : public Figure {
    protected:
        Point2D start,  /* The starting point of the segment. */
                end;    /* The ending point of the segment. */
    public:
        Segment2D(Point2D sp, Point2D ep);
        void print();
};

/**
 * The class to draw polygons.
 */
class Polygon2D : public Figure {
    protected:
        Point2DList pointList;
        bool solid;
    public:
        Polygon2D(Point2DList list);
        Polygon2D(const Point2D * list = NULL, int number = 0);
        virtual void print();
};
#endif

