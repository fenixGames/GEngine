#include "geometry.h"
#include <math.h>
#include <GL/glut.h>
#include <stdio.h>

#define POINT_PREC  180.0f
#define PI  M_PI

using namespace D2D;

/**
 * The constructor of Figure initializes the solid variable to false.
 */
Figure::Figure() 
{
    solid = false;
	mode = GL_LINES;
	transf = Matrix::identity(3);
	org[0] = org[1] = 0;
}

/**
 * Defining the function to set the solid color into the Figure.
 */
void
Figure::setSolid()
{
    solid = true;
}

/**
 * Gets the mode to use to print the figure.
 *
 * @return The mode to use.
 */
GLenum
Figure::getMode()
{
	return mode;
}

/**
 * Rotates a figure so many angles as defined.
 * @param	GLfloat	angle	The angle to rotate the figure.
 */
void
Figure::rotate(GLfloat angle)
{
	angle *= M_PI / 180.0f;
	Matrix rot(3, 3, cos(angle), -sin(angle), 0.0, sin(angle), cos(angle), 0.0, 0.0, 0.0, 1.0), tmp;
	
	tmp = * transf;
	* transf = tmp *  rot;
}

/**
 * Constructor of the 2D Point class.
 *
 * @param   GLint   xx  The horizontal component of the point.
 * @param   GLint   yy  The vertical component of the point.
 */
Point::Point(GLint xx, GLint yy)
{
    x = org[0] = xx;
    y = org[1] = yy;
	mode = GL_POINTS;
}

/**
 * Transforms the point locally and returns the result.
 *
 * @return	The transformed point.
 */
Point *
Point::transform(Figure * ptr)
{
	Vector vect, out;

	vect = Vector(3, (double)x - ptr->org[0], (double) y - ptr->org[1], 1.0);
	out = (*ptr->transf) * vect;

	ptr->transf->print();
	printf("point (%d, %d) org (%d, %d)\n", x, y, ptr->org[0], ptr->org[1]);
	printf("in = (%d, %d), out = (%f, %f)\n", x - ptr->org[0], y - ptr->org[1],
				   	out.getElement(0), out.getElement(1));
	return new Point(out.getElement(0) + ptr->org[0], out.getElement(1) + ptr->org[1]);
}


/**
 * Prints a 2D point on the screen.
 */
Point2DList *
Point::print()
{
    Point2DList * list = new Point2DList();

    list->push_back(transform(this));

    return list;
}

/**
 * Overriding the '=' operator for points.
 *
 * @param   Point2D point   The point to be copied.
 * @return The new point created.
 */
Point
Point::operator = (Point point)
{
    x = point.x;
    y = point.y;

    return *this;
}

/**
 * Calculates the distance between two points.
 * @param   Point2D p1  The first point.
 * @param   Point2D p2  The second point.
 * @return The distance between the two points.
 */
float
Point::distance(Point p1, Point p2)
{
    float xcomp, ycomp;

    xcomp = (p1.x - p2.x);
    ycomp = (p1.y - p2.y);

    xcomp *= xcomp;
    ycomp *= ycomp;

    return sqrt(xcomp + ycomp);
}

/**
 * Transformates a point using the transformation matrix.
 * @param	Matrix	transf	The transformation matrix.
 * @return	The transformated point.
 */
Point *
Point::operator * (Matrix * transf)
{
	Vector in(3, (double) x, (double) y, 1.0), 
		   out(3, .0, .0, .0);

	out = (* transf) * in;

	return new Point(out.getElement(0), out.getElement(1));
}

/**
 * Contructor of the arc class.
 * @param   Point2D     c   The center of the circunference associated to the arc.
 * @param   Point2D     s   The start point of the arc.
 * @param   GLfloat     a   The angle of the arc.
 */
Arc::Arc(Point c, Point s, GLfloat a)
{
    center = c;
    start = s;
    angle = a;

	org[0] = c.x;
	org[1] = c.y;
    /* The angle must be defined between 0 and 2 * PI, so we calculate the equivalent one. */
    while (angle > 360.0f)
        angle -= 360.0f;

    while (angle < 0)
        angle += 360.0f;

	if (angle == 360.0f)
		mode = GL_LINE_LOOP;
}

/**
 * Prints the arc on the screen, after converting each point, of course.
 */
Point2DList *
Arc::print() {
    GLfloat ang_step = PI / POINT_PREC, ang;
    GLfloat rad = Point::distance(start, center);
    GLfloat vpx, vpy;
    Point2DList * list = new Point2DList();

    /* Calculating the initial angle. */
    if (start.x != 0)
        ang = atan(start.y / start.x);
    else if (start.y > 0)
        ang = PI / 2;
    else
        ang = 3 * PI / 2;

    /* Calculating and printing the required points. */
    while (ang < angle * PI / 180.0f) {
        vpx = rad * cos(ang) + center.x;
        vpy = rad * sin(ang) + center.y;

	   	list->push_back(Point(vpx, vpy).transform(this));
        ang += ang_step;
    }
    return list;
}

/**
 * Retrives the ending point of the arc.
 *
 * @return The ending point of the arc.
 */
Point *
Arc::getEnd()
{
    GLfloat radius, /* The radius of the circunference of the Arc. */
            sangle; /* The starting angle of the arc. */
    Point * end;
    GLint   x, y;

    /* Getting the radius of the arc. */
    radius = Point::distance(start, center);

    /* Getting the initial angle. */
    if (start.x != 0)
        sangle = atan(start.y / start.x);
    else if (start.y > 0)
        sangle = PI / 2;
    else
        sangle = 3 * PI / 2;

    /* Getting the coordinates of the new point. */
    x = center.x + radius * cos(sangle + (angle * PI / 180.0f));
    y = center.y + radius * sin(sangle + (angle * PI / 180.0f));

    end = new Point(x, y);

    return end;
}

/** 
 * Constructor of the sector, which is an extension of an arc.
 */
Sector::Sector(Point c, Point s, GLfloat a) : Arc(c, s, a)
{
	mode = GL_LINE_LOOP;
}

/**
 * Prints the sector on the screen.
 */
Point2DList *
Sector::print()
{
    Point2DList * list;

    /* Making the arc for the sector. */
    Arc   arc(center,  start, angle);

    list = arc.print();
    list->push_front(Point(center.x, center.y).transform(this));

    return list;
}


/**
 * Constructor of the circuference, which is an extension of an arc.
 */
Circle::Circle(Point c, GLint radius) : Arc(c, Point(c.x + radius, c.y), 360.0f)
{
}

/**
 * Creates a segment which it is a line between two 2D points.
 *
 * @param   Point sp  The starting point.
 * @param   Point ep  The ending point.
 */
Segment::Segment(Point sp, Point ep)
{
    start = sp;
    end = ep;

	org[0] = (sp.x + ep.x) / 2.0;
	org[1] = (sp.y + ep.y) / 2.0;
}

/**
 * Prints the segment on the screen.
 */
Point2DList *
Segment::print()
{
    Point2DList * list = new Point2DList();

    list->push_back(Point(start.x, start.y).transform(this));
    list->push_back(Point(end.x, end.y).transform(this));

    return list;
}

/**
 * Creating a polygon object through a list of points.
 *
 * @param   Point2DList list    The list of 2D points that makes the polygon.
 */
Polygon::Polygon(Point2DList list)
{
    Point2DList::iterator   iter;

    for (iter = list.begin(); iter != list.end(); iter++)
        pointList.push_back(*iter);
	mode = GL_LINE_LOOP;
}

/**
 * Creating a polygon object though an array of points.
 *
 * @param   Point2D *   list    The array of 2D points that makes the polygon.
 * @param   int         number  The length of the array.
 */
Polygon::Polygon(const Point ** list, int number)
{
    int idx;

    for (idx = 0; idx < number; idx++)
        pointList.push_back((Point *)list[idx]);
	mode = GL_LINE_LOOP;
}

/**
 * Prints the polygon on the screen.
 */
Point2DList *
Polygon::print()
{
    Point2DList::iterator   iter;
    Point2DList * list = new Point2DList();
    Point * point;

    /* Calculating the normalized points of the polygon in order to print it. */
    for (iter = pointList.begin(); iter != pointList.end(); iter++) {
        point = *iter;

        list->push_back(Point(point->x, point->y).transform(this));
    }

    return list;
}

/**
 * Creates an ellipse or a segment of an ellipse.
 *
 * @param   Point2D cen     The center of the ellyse to print.
 * @param   Point2D st      The starting point of the ellypse.
 * @param   GLfloat a       The modifier of the X component.
 * @param   GLfloat b       The modifier of the Y component.
 * @param   GLfloat ang     The angle of the arc.
 */
EllArc::EllArc(Point cen, Point st, GLfloat a, GLfloat b, GLfloat ang)
{
    center = cen;
    start = st;
    xMod = a;
    yMod = b;
    angle = ang;

    /* Sanitizing the angle. */
    while (angle > 360.0f)
        angle -= 360.0f;
    while (angle < 0)
        angle += 360.0f;

	if (angle == 360.0f)
		mode = GL_LINE_LOOP;
}

/**
 * Prints the ellipse on the screen.
 */
Point2DList *
EllArc::print()
{
    GLfloat ang_step = PI / POINT_PREC, ang, end_ang;
    GLfloat vpx, vpy;
    Point2DList *list = new Point2DList();

    /* Calculating the initial angle. */
    if (start.x != 0)
        ang = atan(start.y / start.x);
    else if (start.y > 0)
        ang = PI / 2.0f;
    else
        ang = 1.5f * PI;
	end_ang = angle * PI / 180.0f;

    /* Calculating the points and printing them. */
    while (ang < end_ang) {
        vpx = xMod * cos(ang) + center.x;
        vpy = yMod * sin(ang) + center.y;

        list->push_back(Point(vpx, vpy).transform(this));
        ang += ang_step;
    }

    return list;
}

/**
 * Gets the ending point of the ellipsoidal arc.
 *
 * @return The requested point.
 */
Point *
EllArc::getEnd()
{
	GLfloat sangle; /* The starting angle. */
	GLfloat xcomp, ycomp;

	/* Calculating the starting angle */
	if (start.x != 0)
		sangle = atan(start.y / start.x);
	else if (start.y > 0)
		sangle = PI / 2;
	else
		sangle = 1.5 * PI;

	/* Getting the coordinates. */
	xcomp = xMod * cos( sangle + (angle * PI / 180.0f)) + center.x;
	ycomp = yMod * sin( sangle + (angle * PI / 180.0f)) + center.y;

	return new Point(xcomp, ycomp);
}

/**
 * Constructor of the ellipse. Dummy class. 
 */
Ellipse::Ellipse(Point center, GLfloat a, GLfloat b) : 
	EllArc(center, Point(center.x + a, center.y), a ,b, 360.0f)
{
}

/**
 * Constructor of the regular polygon.
 *
 * @param Point 	center	The center of the circunscribed circunference.
 * @param unsigned	sides	The number of sides of the polygon.
 * @param GLint		rad		The radius of the circunscribed circunference.
 */
RegPol::RegPol(Point center, unsigned int sides, GLint rad)
{
	GLfloat ang_step;
	unsigned int idx;

	if (sides == 0)
		return;

	ang_step = 360.0f / sides;

	for (idx = 0; idx < sides; idx++) {
		pointList.push_back(
				new Point(
					center.x + rad * cos(idx * ang_step * PI / 180.0f),
				   	center.y + rad * sin(idx * ang_step * PI / 180.0f)
					)
				);
	}
	mode = GL_LINE_LOOP;
}

/**
 * Constructor of the rectangle.
 *
 * @param Point	p1	The first point of the rectangle.
 * @param Point	p2	The second point of the rectangle.
 */
Rectangle::Rectangle(Point p1, Point p2)
{
	pointList.push_back(new Point(p1.x, p1.y));
	pointList.push_back(new Point(p1.x, p2.y));
	pointList.push_back(new Point(p2.x, p2.y));
	pointList.push_back(new Point(p2.x, p1.y));
	mode = GL_LINE_LOOP;
}
