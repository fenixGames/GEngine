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

#define Point2DList std::list<D2D::Point *>

/* The following declarations are needed for the Figure part. */
namespace D2D {
    class Figure;
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
};

namespace D3D {
    class Point;
};

/**
 * The abstract class for the printable objects.
 */
class D2D::Figure {
    protected:
        bool solid; /* Indicates if the figure has solid color. */
		GLfloat rotAngle; /* The angle used to rotate the Figure. */
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
class D2D::Point : public D2D::Figure {
    public:
        /* These components are public in order to allow all the figures access them. */
        GLint   x, /* The horizontal component of the point. */
                y; /* The vertical component of the point. */

        Point(GLint xx = 0, GLint yy = 0);

        /* Setting the assignator between two points. */
        D2D::Point operator = (D2D::Point);

        /* Calculates the distance between two points. */
        static float distance(D2D::Point p1, D2D::Point p2);

        /* Prints the figure. */
        virtual void print();
};

/**
 * The class used to print a 3D point.
 */
class D3D::Point : public D2D::Point {
    protected:
        GLint z; /* The depth component of the point. */
    public:
        Point(GLint xx, GLint yy, GLint zz);

        /* TODO Prints the 3D object. */
        void print();
};

/**
 * The class used to print an arc.
 */
class D2D::Arc : public D2D::Figure {
    protected:
        D2D::Point center; /* The point associated to the center of the arc. */
        D2D::Point start;  /* The starting point of the arc. */
        GLfloat angle;  /* The angle associated to the arc. */

    public:
        Arc(D2D::Point c, D2D::Point s, GLfloat a = 360.0f);

        /* Gets the ending point of the arc. */
        D2D::Point * getEnd();

        /* Prints the arc. */
        virtual void print();
};

/**
 * The class used to print a circular sector.
 */
class D2D::Sector : public D2D::Arc {
    public:
        Sector(D2D::Point c, D2D::Point s, GLfloat a = 360.0f);

        /* Prints the sector. */
        void print();
};

/**
 * The class to print a circunference.
 */
class D2D::Circle : public D2D::Arc {
    public:
        Circle(D2D::Point c, GLint radius);
};

/**
 * The class used to print a segment.
 */
class D2D::Segment : public D2D::Figure {
    protected:
        D2D::Point start,  /* The starting point of the segment. */
                end;    /* The ending point of the segment. */
    public:
        Segment(D2D::Point sp, D2D::Point ep);

        /* Prints the segment on the screen. */
        void print();
};

/**
 * The class to draw polygons.
 */
class D2D::Polygon : public D2D::Figure {
    protected:
        Point2DList pointList;  /* The list of points associated to the polygon. */
    public:
        /* There are two ways of create the Polygon, though a list of points or through an array. */
        Polygon(Point2DList list);
        Polygon(const D2D::Point ** list = NULL, int number = 0);

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
class D2D::EllArc : public D2D::Figure {
    protected:
        D2D::Point center, /* The center of the associated ellipse. */
                start;  /* The starting point used to draw the ellipse. */
        GLfloat xMod,   /* The modifier of the X component of the ellipse. */
                yMod;   /* The modifier of the Y component of the ellipse. */
        GLfloat angle;  /* The angle of the arc to be drawn. */
    public:
        EllArc(D2D::Point center, D2D::Point start, GLfloat a, GLfloat b, GLfloat angle = 360.0f);

        /* Prints the ellipsoidal arc on the screen. */
        void print();

		/* Gets the ending point of the ellipsoidal arc. */
		D2D::Point * getEnd();
};

/**
 * The class to expand the ellipsoidal arc to an ellipse.
 */
class D2D::Ellipse : public D2D::EllArc {
	public:
		Ellipse(D2D::Point center, GLfloat a, GLfloat b);
};

/**
 * Creating the regular polygon class.
 */
class D2D::RegPol : public D2D::Polygon {
	public:
		RegPol(D2D::Point center, unsigned int sides, GLint rad);
};

/**
 * The rectangle class to draw a rectangle.
 *
 * It will be drawn using the following expression:
 * 	P1 = (P1.x, P1.y)
 * 	P2 = (P1.x, P2.y)
 * 	P3 = (P2.x, P2.y)
 * 	P4 = (P2.x, P1.y)
 */
class D2D::Rectangle : public D2D::Polygon {
	public:
		Rectangle(D2D::Point p1, D2D::Point p2);
};

#endif

