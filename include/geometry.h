/**
 * This file contains everything about geometrical classes to be printed on the screen.
 *
 * @author  Roberto Fernandez Cueto
 * @date    12.10.2015
 *
 * $Id$
 */

#ifndef _GEOMETRY_H
#define _GEOMETRY_H

#include <list>
#include <GL/gl.h>
#include "matrix.h"

#define PointList 	std::list<Point *>
#define MeshList	std::list<Mesh *>

/* The following declarations are needed for the Figure part. */

#ifndef _GENGINE_H
namespace GEngine {
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
};
#endif

/**
 * The abstract class for the printable objects.
 * TODO: Calculate the distance to the camera (Point)
 */
class GEngine::Geometry::Figure {
		friend class Point;
    protected:
        bool solid; /* Indicates if the figure has solid color. */
		GLenum	mode;	/* Indicates the mode to use to print. */
		GLfloat	angle[3];	/* The angles used in the rotation (yaw, pitch, roll). */
		GLdouble color[3]; /* The color of the figure, some figures could have complex coloring to 
                              be developed by the user. */
        PointList * buffer;   /* The calculated list of points to be printed. Usefull for movement. */
    public:
		int		org[3];		/* The local origin of coordinates for the figure. */
	
        Figure();
		Figure(const Figure& fig);
        ~Figure();

        /* Virtual functions needed to be overriden. */
        virtual PointList * print() = 0; 
        virtual void motion(int time) = 0;

        /* By default, all the figures are wired figures, with this, they will be solid. */
        void setSolid();

        /* Returns whether the figure is solid or not. */
        bool getSolid();

        /* Rotates the figure the angle declared in degrees for the yaw, pitch and roll angles. */
        void rotate(GLfloat yaw, GLfloat pitch = 0, GLfloat roll = 0);

		/* Returns the mode to be used to print the figure. */
		GLenum getMode();

		/* Sets the color for the figure. */
		void setColor(GLdouble red, GLdouble green, GLdouble blue);

		/* Gets the corresponding color. */
		GLdouble getRed();
		GLdouble getGreen();
		GLdouble getBlue();

		/* Copy the figure. */
		Figure& operator = (const Figure& fig);
};

/**
 * Class for static figures, with no motion.
 */
class GEngine::Geometry::StaticFigure : public GEngine::Geometry::Figure {
    public:
        void motion(int time);
};

/**
 * The class used to print a 2D point.
 */
class GEngine::Geometry::Point  {
    public:
        /* These components are public in order to allow all the figures access them. */
        GLdouble    x, /* The horizontal component of the point. */
                    y, /* The vertical component of the point. */
                    z; /* The depth component of the point. */

        Point(GLdouble xx = 0, GLdouble yy = 0, GLdouble zz = 0);

        /* Setting the assignator between two points. */
        Point operator = (Point);

        /* Calculates the distance between two points. */
        static float distance(Point p1, Point p2);

		/* Transforms the point locally. */
        Point * transform(Figure * ptr);

        /* Calculates the point between another two using a time parameter. */
        static Point * tween(const Point p1, const Point p2, double time);
};

/**
 * The class used to print an arc.
 */
class GEngine::Geometry::Arc : public GEngine::Geometry::StaticFigure {
    protected:
        Point center; /* The point associated to the center of the arc. */
        Point start;  /* The starting point of the arc. */
        GLfloat angle;  /* The angle associated to the arc. */

    public:
        Arc(Point c, Point s, GLfloat a = 360.0f);

        /* Gets the ending point of the arc. */
        Point * getEnd();

        /* Prints the arc. */
        virtual PointList * print();
};

/**
 * The class used to print a circular sector.
 */
class GEngine::Geometry::Sector : public GEngine::Geometry::Arc {
    public:
        Sector(Point c, Point s, GLfloat a = 360.0f);

        /* Prints the sector. */
        PointList * print();
};

/**
 * The class to print a circunference.
 */
class GEngine::Geometry::Circle : public GEngine::Geometry::Arc {
    public:
        Circle(Point c, GLint radius);
};

/**
 * The class used to print a segment.
 */
class GEngine::Geometry::Segment : public GEngine::Geometry::StaticFigure {
    protected:
        Point   start,  /* The starting point of the segment. */
                end;    /* The ending point of the segment. */
    public:
        Segment(Point sp, Point ep);

        /* Prints the segment on the screen. */
        PointList * print();
};

/**
 * The class to draw polygons.
 */
class GEngine::Geometry::Polygon : public GEngine::Geometry::StaticFigure {
    protected:
        PointList pointList;  /* The list of points associated to the polygon. */
    public:
        /* There are two ways of create the Polygon, though a list of points or through an array. */
        Polygon(PointList list);
        Polygon(const Point ** list = NULL, int number = 0);

        /* Prints the polygon on the screen. */
        virtual PointList * print();
};

/**
 * The class used to print an ellipsoidal arc (A segment of an ellipse).
 *
 * The points will be drawn using the next convention:
 *  x = xMod * cos(ang);
 *  y = yMod * sin(ang);
 */
class GEngine::Geometry::EllArc : public GEngine::Geometry::StaticFigure {
    protected:
        Point   center, /* The center of the associated ellipse. */
                start;  /* The starting point used to draw the ellipse. */
        GLfloat xMod,   /* The modifier of the X component of the ellipse. */
                yMod;   /* The modifier of the Y component of the ellipse. */
        GLfloat angle;  /* The angle of the arc to be drawn. */
    public:
        EllArc(Point center, Point start,
                GLfloat a, GLfloat b, GLfloat angle = 360.0f);

        /* Prints the ellipsoidal arc on the screen. */
        PointList * print();

		/* Gets the ending point of the ellipsoidal arc. */
		Point * getEnd();
};

/**
 * The class to expand the ellipsoidal arc to an ellipse.
 */
class GEngine::Geometry::Ellipse : public GEngine::Geometry::EllArc {
	public:
		Ellipse(Point center, GLfloat a, GLfloat b);
};

/**
 * Creating the regular polygon class.
 */
class GEngine::Geometry::RegPol : public GEngine::Geometry::Polygon {
	public:
		RegPol(Point center, unsigned int sides, GLint rad);
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
class GEngine::Geometry::Rectangle : public GEngine::Geometry::Polygon {
	public:
        Rectangle(Point p1, Point p2);
};

class GEngine::Geometry::Mesh {
	protected:
		Vector	normal;
		Point	point;
	public:
		Mesh(Vector vect, Point v);
		Mesh(const Mesh& mesh);
		Mesh(Point points[3]);

		/* Calculates the intersection of the three meshes. */
		static Point * intersection(const Mesh m1, const Mesh m2, const Mesh m3);
};

/**
 * Defining a polyhedron.
 */
class GEngine::Geometry::Polyhedron : public GEngine::Geometry::StaticFigure {
    private:
        void getPoints(MeshList * list);
    protected:
        PointList   * pointList;
    public:
        Polyhedron(MeshList list);
        Polyhedron(PointList list);
        Polyhedron(Point ** list, unsigned int npoints);
        Polyhedron(Mesh ** list = NULL, unsigned int nmeshes = 0);
		~Polyhedron();

        /* Prints the polyhedra on the screen. */
        virtual PointList * print();
};

/** TODO 3D Figures:
 *  `- Prisms.
 *  `- Regular polyhedra. 
 *    (Tetrahedron, Cube, Octahedron, Dodecahedron, Icosahedron)
 *  `- Pyramids.
 *  - Spherical plane.
 *  `- Spheres.
 *  - Ellipsoid.
 *  `- Ellipsoidal plane.
 *  - Cilinders.
 *  - Cones.
 *  - Toroid.
 */
#endif

