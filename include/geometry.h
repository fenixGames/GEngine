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
        bool solid; /* Indicates if the figure has solid color. */
    public:
        Figure();

        /* Virtual function needed to be overriden. */
        virtual void print() = 0; 

        /* By default, all the figures are wired figures, with this, they will be solid. */
        void setSolid();
};

/**
 * The class used to print a 2D point.
 */
class Point2D : public Figure {
    public:
        /* These components are public in order to allow all the figures access them. */
        GLint   x, /* The horizontal component of the point. */
                y; /* The vertical component of the point. */

        Point2D(GLint xx = 0, GLint yy = 0);

        /* Setting the assignator between two points. */
        Point2D operator = (Point2D);

        /* Calculates the distance between two points. */
        static float distance(Point2D p1, Point2D p2);

        /* Prints the figure. */
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

        /* TODO Prints the 3D object. */
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

        /* Gets the ending point of the arc. */
        Point2D * getEnd();

        /* Prints the arc. */
        virtual void print();
};

/**
 * The class used to print a circular sector.
 */
class Sector2D : public Arc2D {
    public:
        Sector2D(Point2D c, Point2D s, GLfloat a = 2 * PI);

        /* Prints the sector. */
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

        /* Prints the segment on the screen. */
        void print();
};

/**
 * The class to draw polygons.
 */
class Polygon2D : public Figure {
    protected:
        Point2DList pointList;  /* The list of points associated to the polygon. */
    public:
        /* There are two ways of create the Polygon, though a list of points or through an array. */
        Polygon2D(Point2DList list);
        Polygon2D(const Point2D ** list = NULL, int number = 0);

        /* Prints the polygon on the screen. */
        virtual void print();
};

/**
 * The class used to print an ellipsoidal arc (A segment of an ellipse).
 *
 * The points will be drawn using the next convention:
 *  x = xMod * cos(ang);
 *  y = yMod * sin(ang);
 */
class EllArc2D : public Figure {
    protected:
        Point2D center, /* The center of the associated ellipse. */
                start;  /* The starting point used to draw the ellipse. */
        GLfloat xMod,   /* The modifier of the X component of the ellipse. */
                yMod;   /* The modifier of the Y component of the ellipse. */
        GLfloat angle;  /* The angle of the arc to be drawn. */
    public:
        EllArc2D(Point2D center, Point2D start, GLfloat a, GLfloat b, GLfloat angle = 2 * PI);

        /* Prints the ellipsoidal arc on the screen. */
        void print();
};

#endif

