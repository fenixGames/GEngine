/**
 * This file contains everything about 2D geometrical figures to be printed on the screen.
 *
 * @author  Roberto Fernandez Cueto
 * @date    12.10.2015
 *
 * $Id$
 */


#include "geometry.h"
#include <math.h>
#include <GL/glut.h>
#include <string.h>

#define POINT_PREC  1800.0f /* 10 points per grad. */
#define PI  M_PI

using namespace GEngine::Geometry;

/**
 * The constructor of Figure initializes the solid variable to false.
 */
Figure::Figure() 
{
    solid = false;
	mode = GL_LINES;
    material = NULL;
    memset(angle, 0, sizeof(GLfloat) * 3);
    memset(org, 0, sizeof(int) * 3);
}

Figure::Figure(const Figure& fig)
{
	solid = fig.solid;
	mode = fig.mode;
    memcpy(angle, fig.angle, 3 * sizeof(GLfloat));
	memcpy(org, fig.org, 2* sizeof(int));

    if (fig.material != NULL)
        material = fig.material;
    else
        material = NULL;
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
 * @param	GLfloat	yaw	    The angle for the yaw of the figure.
 * @param	GLfloat	pitch   The angle for the pitch of the figure.
 * @param	GLfloat	roll    The angle for the roll of the figure.
 */
void
Figure::rotate(GLfloat yaw, GLfloat pitch, GLfloat roll)
{
	angle[0] = yaw * M_PI / 180.0f;
	angle[1] = pitch * M_PI / 180.0f;
	angle[2] = roll * M_PI / 180.0f;
}

/**
 * Sets the material for the figure using a pointer to a material class.
 * @param   Material    mat     The material to use.
 */
void
Figure::setMaterial(Material * mat)
{
    material = mat;
}

/**
 * Sets the material from a texture stored on a file.
 * @param   char    *file   The path to the file where the texture is stored.
 */
void
Figure::setMaterialFromFile(const char * file)
{
    material = new Material(Material::GL_FROM_FILE, (void *)file);
}

/**
 * Sets the material from a pixmap structure.
 * @param   Pixmap  pixmap  The pixel map where the info about the texture is stored.
 */
void
Figure::setMaterialFromPixMap(struct Pixmap pixmap)
{
    material = new Material(Material::GL_FROM_PIXELMAP, (void *)&pixmap);
}

/**
 * Sets the material using the RGB color.
 * @param   GLubyte     red     The red byte of the color.
 * @param   GLubyte     green     The green byte of the color.
 * @param   GLubyte     blue     The blue byte of the color.
 */
void
Figure::setMaterialFromRGB(GLubyte red, GLubyte green, GLubyte blue)
{
    GLubyte color[3] = { red, green, blue };

    material = new Material(Material::GL_SOLID_COLOR, (void *)color);
}

/**
 * Activates the material if set, if not, it will be a wired figure that can be solid.
 * That means that the back faces will not be shown.
 */
void
Figure::activeMaterial()
{
    /* Check if the material is set or not. */
    if (material != NULL) {
        glPolygonMode(GL_FRONT, GL_FILL);
        glPolygonMode(GL_BACK, GL_POINT);
        material->activate();
    } else if (solid) {
        glPolygonMode(GL_FRONT, GL_LINE);
        glPolygonMode(GL_BACK, GL_POINT);
    } else
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}

/**
 * Deactivates the material if it was set.
 */
void
Figure::deactivateMaterial()
{
    if (material != NULL)
        material->deactivate();
}

/**
 * Copy asignment, copies the figure.
 * @param	Figure	fig		The figure to copy.
 * @return	The copied figure.
 */
Figure&
Figure::operator = (const Figure& fig)
{
	solid = fig.solid;
	mode = fig.mode;
    memcpy(angle, fig.angle, 3 * sizeof(GLfloat));
	memcpy(org, fig.org, 2 * sizeof(int));

	return * this;
}

/**
 * Sets the motion for a static figure, that means, it just returns.
 * @param   int     time    The time for the motion, unused for static.
 */
void
StaticFigure::motion(int time_stamp)
{
}

/**
 * Constructor of the 2D Point class.
 *
 * @param   GLint   xx  The horizontal component of the point.
 * @param   GLint   yy  The vertical component of the point.
 */
Point::Point(GLdouble xx, GLdouble yy, GLdouble zz, GLdouble ss, GLdouble tt)
{
    x = xx;
    y = yy;
    z = zz;
    s = ss;
    t = tt;
}

/**
 * Transforms the point locally and returns the result.
 *
 * @return	The transformed point.
 */
Point *
Point::transform(Figure * ptr)
{
	Point * end;
    GLfloat * angle = ptr->angle;
    Vector in(3, x - ptr->org[0], y - ptr->org[1], z - ptr->org[2]),
           out(3, 0.0, 0.0, 0.0);
    /* The three rotational matrix, starting for roll. */
    Matrix roll(3, 3, 
            cos(angle[0]), -sin(angle[0]), 0.0, 
            sin(angle[0]), cos(angle[0]), 0.0, 
            0.0, 0.0, 1.0),
           pitch(3, 3,
                   cos(angle[1]), 0.0, sin(angle[1]),
                   0.0, 1.0, 0.0,
                   -sin(angle[1]), 0.0, cos(angle[1])),
           yaw(3, 3,
                   1.0, 0.0, 0.0,
                   0.0, cos(angle[2]), -sin(angle[2]),
                   0.0, sin(angle[2]), cos(angle[2])), rotation = Matrix::identity(3);

    rotation = roll * pitch * yaw;

    out = rotation * in;

    end = new Point(out.getElement(0) + ptr->org[0], out.getElement(1) + ptr->org[1],
            out.getElement(2) + ptr->org[2]);
	return end;
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
    z = point.z;
    s = point.s;
    t = point.t;

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
    float xcomp, ycomp, zcomp;

    xcomp = (p1.x - p2.x);
    ycomp = (p1.y - p2.y);
    zcomp = (p1.z - p2.z);

    xcomp *= xcomp;
    ycomp *= ycomp;
    zcomp *= zcomp;

    return sqrt(xcomp + ycomp + zcomp);
}

/**
 * Calculates the point between two other points using the time as a parameter.
 * This function is usefull for motion.
 * For time values from 0 to 1 are for interpolation, for negative values are used to
 * extrapolate from the end point to the initial, and values greater than 1, are for
 * extrapolation in the other direction.
 * @param   Point   p1      The initial point.
 * @param   Point   p2      The final point.
 * @param   double  time    The time parameter.
 * @return  The calculated point between the initial and the final at the time "time".
 */
Point *
Point::tween(const Point p1, const Point p2, double time)
{
    int xcalc, ycalc, zcalc;

    xcalc = (1.0 - time) * p1.x + time * p2.x;
    ycalc = (1.0 - time) * p1.y + time * p2.y;
    zcalc = (1.0 - time) * p1.z + time * p2.z;

    return new Point(xcalc, ycalc, zcalc);
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
    org[2] = c.z;
    /* The angle must be defined between 0 and 2 * PI, so we calculate the equivalent one. */
    while (angle > 360.0f)
        angle -= 360.0f;

    while (angle < 0)
        angle += 360.0f;

	if (angle == 360.0f)
		mode = GL_POLYGON;
}

/**
 * Prints the arc on the screen, after converting each point, of course.
 */
PointList *
Arc::print() {
    GLfloat ang_step = PI / POINT_PREC, ang, end_angle;
    GLfloat rad = Point::distance(start, center);
    GLfloat vpx, vpy;
    PointList * list = new PointList();

    /* Calculating the initial angle. */
    if (start.x != 0)
        ang = atan(start.y / start.x);
    else if (start.y > 0)
        ang = PI / 2;
    else
        ang = 3 * PI / 2;

    end_angle = (angle * M_PI / 180.0f) + ang ;
    /* Calculating and printing the required points. */
    while (ang < end_angle) {
        vpx = rad * cos(ang) + center.x;
        vpy = rad * sin(ang) + center.y;

	   	list->push_back(Point(vpx, vpy, center.z, 0.5 * cos(angle), 0.5 * sin(angle)).transform(this));
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

    end = new Point(x, y, center.z);

    return end;
}

/** 
 * Constructor of the sector, which is an extension of an arc.
 */
Sector::Sector(Point c, Point s, GLfloat a) : Arc(c, s, a)
{
	mode = GL_POLYGON;
}

/**
 * Prints the sector on the screen.
 */
PointList *
Sector::print()
{
    PointList * list;

    /* Making the arc for the sector. */
    Arc   arc(center,  start, angle);

    list = arc.print();
    list->push_front(Point(center.x, center.y, center.z, 0.5, 0.5).transform(this));

    return list;
}


/**
 * Constructor of the circuference, which is an extension of an arc.
 */
Circle::Circle(Point c, GLint radius) : Arc(c, Point(c.x + radius, c.y, c.z), 360.0f)
{
    mode = GL_POLYGON;
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
    org[2] = (sp.z + ep.z) / 2.0;
}

/**
 * Prints the segment on the screen.
 */
PointList *
Segment::print()
{
    PointList * list = new PointList();

    list->push_back(Point(start.x, start.y, start.z, 0.0, 0.0).transform(this));
    list->push_back(Point(end.x, end.y, end.z, 1.0, 1.0).transform(this));

    return list;
}

/**
 * Creating a polygon object through a list of points.
 *
 * @param   Point2DList list    The list of 2D points that makes the polygon.
 */
Polygon::Polygon(PointList list)
{
    PointList::iterator   iter;

    for (iter = list.begin(); iter != list.end(); iter++)
        pointList.push_back(*iter);
	mode = GL_POLYGON;

    getOrigin();
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
	mode = GL_POLYGON;

    getOrigin();
}

/**
 * Searchs for the center of the polygon. Used to calculate the distance to the camera.
 */
void
Polygon::getOrigin()
{
    float dist, max_dist = 0.0f;
    PointList::iterator ita, itb;

    /* Looking for the maximal distance between points, which will generate the diameter 
     * of the polygon. */
    for (ita = pointList.begin(); ita != pointList.end(); ita++) {
        itb = ita;
        for (itb++; itb != pointList.end(); itb++) {
            dist = Point::distance(*(*ita), *(*itb));
            if (dist > max_dist) {
                /* Setting the origin. */
                org[0] = ((*ita)->x + (*itb)->x) / 2.0;
                org[1] = ((*ita)->y + (*itb)->y) / 2.0;
                org[2] = ((*ita)->z + (*itb)->z) / 2.0;
                max_dist = dist;
            }
        }
    }
    radius = max_dist;
}

/**
 * Prints the polygon on the screen.
 */
PointList *
Polygon::print()
{
    PointList::iterator   iter;
    PointList * list = new PointList();
    Point * point;
    GLdouble tex[2];

    /* Calculating the normalized points of the polygon in order to print it. */
    for (iter = pointList.begin(); iter != pointList.end(); iter++) {
        point = *iter;

        /* Calculating the texture geometry. */
        tex[0] = 0.5 * (1 + ((point->x - org[0]) / radius));
        tex[1] = 0.5 * (1 + ((point->y - org[1]) / radius));

        list->push_back(Point(point->x, point->y, point->z, tex[0], tex[1]).transform(this));
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
	org[0] = cen.x;
	org[1] = cen.y;
    org[2] = cen.z;

    /* Sanitizing the angle. */
    while (angle > 360.0f)
        angle -= 360.0f;
    while (angle < 0)
        angle += 360.0f;

	if (angle == 360.0f)
		mode = GL_POLYGON;
}

/**
 * Prints the ellipse on the screen.
 */
PointList *
EllArc::print()
{
    GLfloat ang_step = PI / POINT_PREC, ang, end_ang;
    GLfloat vpx, vpy;
    PointList *list = new PointList();
    GLdouble tex[2];

    /* Calculating the initial angle. */
    if (start.x != 0)
        ang = atan(start.y / start.x);
    else if (start.y > 0)
        ang = PI / 2.0f;
    else
        ang = 1.5f * PI;
	end_ang = angle * PI / 180.0f + ang;

    /* Calculating the points and printing them. */
    while (ang < end_ang) {
        vpx = xMod * cos(ang) + center.x;
        vpy = yMod * sin(ang) + center.y;

        tex[0] = 0.5 * cos(ang);
        tex[1] = 0.5 * sin(ang);

        list->push_back(Point(vpx, vpy, center.z, tex[0], tex[1]).transform(this));
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

	return new Point(xcomp, ycomp, center.z);
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
	mode = GL_POLYGON;

    org[0] = center.x;
    org[1] = center.y;
    org[2] = center.z;
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

    org[0] = (p1.x + p2.x) / 2.0;
    org[1] = (p1.y + p2.y) / 2.0;
    org[2] = (p1.z + p2.z) / 2.0;
}

