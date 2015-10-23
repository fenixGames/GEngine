#include "geometry.h"
#include <math.h>
#include <GL/glut.h>

#define POINT_PREC  180.0f
#define PI  M_PI

using namespace D2D;
/**
 * Static function to make the transformation between the points and the screen.
 *
 * @param[in]   GLint   xin     The x component of the input point.
 * @param[in]   GLint   yin     The y component of the input point.
 * @param[in]   GLint   zin     The z component of the input point.
 * @param[out]  GLfloat *xout   The x component of the output point.
 * @param[out]  GLfloat *yout   The y component of the output point.
 * @param[out]  GLfloat *zout   The z component of the output point.
 */
static void
canvas(GLint xin, GLint yin, GLint zin, GLfloat *xout = NULL, GLfloat *yout = NULL, GLfloat *zout = NULL)
{
    GLint wwidth, wheight;
    GLfloat alpha, beta, gamma, delta;

    /* Getting the size of the window to make the transformation. */
    wwidth = glutGet(GLUT_WINDOW_WIDTH);
    wheight = glutGet(GLUT_WINDOW_HEIGHT);

    /* Calculating the modifiers. */
    alpha   = (2.0f / wwidth);
    beta    = -1.0f;
    gamma   = (GLfloat)(2.0f / wheight);
    delta   = -1.0f;

    /* Calculating the new components. */
    if (xout != NULL)
        *xout = alpha * xin + beta;
    if (yout != NULL)
        *yout = gamma * yin + delta;

    /* TODO: Depth component. */
}

/**
 * The constructor of Figure initializes the solid variable to false.
 */
Figure::Figure() 
{
    this->solid = false;
}

/**
 * Defining the function to set the solid color into the Figure.
 */
void
Figure::setSolid()
{
    this->solid = true;
}

/**
 * Constructor of the 2D Point class.
 *
 * @param   GLint   xx  The horizontal component of the point.
 * @param   GLint   yy  The vertical component of the point.
 */
Point::Point(GLint xx, GLint yy)
{
    this->x = xx;
    this->y = yy;
}

/**
 * Prints a 2D point on the screen.
 */
Point2DList *
Point::print()
{
    Point2DList * list = new Point2DList();

    list->push_back(new Point(this->x, this->y));

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
    this->x = point.x;
    this->y = point.y;

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
 * Contructor of the arc class.
 * @param   Point2D     c   The center of the circunference associated to the arc.
 * @param   Point2D     s   The start point of the arc.
 * @param   GLfloat     a   The angle of the arc.
 */
Arc::Arc(Point c, Point s, GLfloat a)
{
    this->center = c;
    this->start = s;
    this->angle = a;

    /* The angle must be defined between 0 and 2 * PI, so we calculate the equivalent one. */
    while (this->angle > 360.0f)
        this->angle -= 360.0f;

    while (this->angle < 0)
        this->angle += 360.0f;
}

/**
 * Prints the arc on the screen, after converting each point, of course.
 */
Point2DList *
Arc::print() {
    GLfloat ang_step = PI / POINT_PREC, ang;
    GLfloat rad = Point::distance(this->start, this->center);
    GLfloat vpx, vpy;
    Point2DList * list;

    /* Calculating the initial angle. */
    if (this->start.x != 0)
        ang = atan(this->start.y / this->start.x);
    else if (this->start.y > 0)
        ang = PI / 2;
    else
        ang = 3 * PI / 2;

    /* Calculating and printing the required points. */
    while (ang < this->angle * PI / 180.0f) {
        vpx = rad * cos(ang) + this->center.x;
        vpy = rad * sin(ang) + this->center.y;

        list->push_back(new Point(vpx, vpy));
        
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
    radius = Point::distance(this->start, this->center);

    /* Getting the initial angle. */
    if (this->start.x != 0)
        sangle = atan(this->start.y / this->start.x);
    else if (this->start.y > 0)
        sangle = PI / 2;
    else
        sangle = 3 * PI / 2;

    /* Getting the coordinates of the new point. */
    x = this->center.x + radius * cos(sangle + (this->angle * PI / 180.0f));
    y = this->center.y + radius * sin(sangle + (this->angle * PI / 180.0f));

    end = new Point(x, y);

    return end;
}

/** 
 * Constructor of the sector, which is an extension of an arc.
 */
Sector::Sector(Point c, Point s, GLfloat a) : Arc(c, s, a)
{
}

/**
 * Prints the sector on the screen.
 */
Point2DList *
Sector::print()
{
    Point2DList * list;

    /* Making the arc for the sector. */
    Arc   arc(this->center, this-> start, this->angle);

    list = arc.print();
    list->push_front(new Point(this->center.x, this->center.y));

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
    this->start = sp;
    this->end = ep;
}

/**
 * Prints the segment on the screen.
 */
Point2DList *
Segment::print()
{
    Point2DList * list = new Point2DList();

    list->push_back(new Point(this->start.x, this->start.y));
    list->push_back(new Point(this->end.x, this->end.y));

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
        this->pointList.push_back(*iter);
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
        this->pointList.push_back((Point *)list[idx]);
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
    for (iter = this->pointList.begin(); iter != this->pointList.end(); iter++) {
        point = *iter;

        list->push_back(new Point(point->x, point->y));
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
    this->center = cen;
    this->start = st;
    this->xMod = a;
    this->yMod = b;
    this->angle = ang;

    /* Sanitizing the angle. */
    while (this->angle > 360.0f)
        this->angle -= 360.0f;
    while (this->angle < 0)
        this->angle += 360.0f;
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
    if (this->start.x != 0)
        ang = atan(this->start.y / this->start.x);
    else if (this->start.y > 0)
        ang = PI / 2.0f;
    else
        ang = 1.5f * PI;
	end_ang = this->angle * PI / 180.0f;

    /* Calculating the points and printing them. */
    while (ang < end_ang) {
        vpx = this->xMod * cos(ang) + this->center.x;
        vpy = this->yMod * sin(ang) + this->center.y;

        list->push_back(new Point(vpx, vpy));
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
	if (this->start.x != 0)
		sangle = atan(this->start.y / this->start.x);
	else if (this->start.y > 0)
		sangle = PI / 2;
	else
		sangle = 1.5 * PI;

	/* Getting the coordinates. */
	xcomp = this->xMod * cos( sangle + (this->angle * PI / 180.0f)) + this->center.x;
	ycomp = this->yMod * sin( sangle + (this->angle * PI / 180.0f)) + this->center.y;

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
		this->pointList.push_back(
				new Point(
					center.x + rad * cos(idx * ang_step * PI / 180.0f),
				   	center.y + rad * sin(idx * ang_step * PI / 180.0f)
					)
				);
	}
}

/**
 * Constructor of the rectangle.
 *
 * @param Point	p1	The first point of the rectangle.
 * @param Point	p2	The second point of the rectangle.
 */
Rectangle::Rectangle(Point p1, Point p2)
{
	this->pointList.push_back(new Point(p1.x, p1.y));
	this->pointList.push_back(new Point(p1.x, p2.y));
	this->pointList.push_back(new Point(p2.x, p2.y));
	this->pointList.push_back(new Point(p2.x, p1.y));
}
