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
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
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
StaticFigure::motion(double time_stamp)
{
}

/**
 * Constructor of the face.
 */
Face::Face(PointList * list, Vector * n)
{
    vertex = new PointList( *list);
    normal = new Vector(*n);
}

/**
 * Destructor.
 */
Face::~Face()
{
    delete vertex;
    delete normal;
}

/**
 * Applies the transformations to the face and returns the new one.
 * @param GLint     center[3]   The center of the face.
 * @param GLfloat   angles[3]   The angles of rotation.
 * @return  The new face.
 */
Face *
Face::transform(GLint center[3], GLfloat angles[3])
{
    PointList::iterator     iter;
    PointList   newVert;
    Vector  in, out;
    Matrix  roll(3, 3,
                    cos(angles[0]), -sin(angles[0]), 0.0,
                    sin(angles[0]), cos(angles[0]), 0.0,
                    0.0, 0.0, 1.0),
            pitch(3, 3,
                    cos(angles[1]), 0.0, sin(angles[1]),
                    0.0, 1.0, 0.0,
                    -sin(angles[1]), 0.0, cos(angles[1])),
            yaw(3, 3,
                    1.0, 0.0, 0.0,
                    0.0, cos(angles[2]), -sin(angles[2]),
                    0.0, sin(angles[2]), cos(angles[2])), rotation;

    /* Setting the rotation matrix. */
    rotation = roll * yaw * pitch;
#ifdef DEBUG
//    rotation.print();
#endif
    /* Going through all the points of the face. */
    for (iter = vertex->begin(); iter != vertex->end(); iter++) {
        in = Vector(3, (*iter)->x - center[0],
                (*iter)->y - center[1], (*iter)->z - center[2]);
        out = rotation * in;

        newVert.push_back(new Point(out[0] + center[0], out[1] + center[1], 
                    out[2] + center[2], (*iter)->s, (*iter)->t));
    }
    out = rotation * (*normal);

    return new Face(&newVert, &out);
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
 * @param   GLfloat     a   The angle of the arc.
 */
Arc::Arc(Point c, Point s, GLfloat a)
{
    GLfloat ang_step = PI / POINT_PREC, ang, end_angle;
    GLfloat rad = Point::distance(s, c);
    GLfloat vpx, vpy;

    /* Setting the center. */
	org[0] = c.x;
	org[1] = c.y;
    org[2] = c.z;

    /* The angle must be defined between 0 and 2 * PI, so we calculate the equivalent one. */
    while (a > 360.0f)
        a -= 360.0f;

    while (a < 0)
        a += 360.0f;

	if (a == 360.0f)
		mode = GL_POLYGON;

    /* Calculating the initial angle. */
    if (s.x != 0)
        ang = atan(s.y / s.x);
    else if (s.y > 0)
        ang = PI / 2;
    else
        ang = 3 * PI / 2;

    end_angle = (a * M_PI / 180.0f) + ang ;
    /* Calculating and printing the required points. */
    while (ang < end_angle) {
        vpx = rad * cos(ang) + c.x;
        vpy = rad * sin(ang) + c.y;

	   	vertices.push_back(new Point(vpx, vpy, c.z, 0.5 * cos(ang), 0.5 * sin(ang)));
        ang += ang_step;
    }

}

/**
 * Prints the arc on the screen, after converting each point, of course.
 */
FaceList *
Arc::print()
{
    Vector dir(3, 0.0, 0.0, 1.0);
    FaceList * list = new FaceList();

    list->push_back(Face(&vertices, &dir).transform(org, angle));

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
    return new Point(*vertices.back());
}

/** 
 * Constructor of the sector, which is an extension of an arc.
 */
Sector::Sector(Point c, Point s, GLfloat a) : Arc(c, s, a)
{
	mode = GL_POLYGON;
    vertices.push_back(new Point(c.x, c.y, c.z, 0.5, 0.5));
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
    
    vertices.push_back(new Point(start.x, start.y, start.z, 0.0, 0.0));
    vertices.push_back(new Point(end.x, end.y, end.z, 1.0, 1.0));
}

/**
 * Prints the segment on the screen.
 */
FaceList *
Segment::print()
{
    FaceList * list = new FaceList();
    Vector dir(3, 0.0, 0.0, 1.0);

    list->push_back(Face(&vertices, &dir).transform(org, angle));

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
        vertices.push_back(*iter);
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
        vertices.push_back((Point *)list[idx]);
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
    for (ita = vertices.begin(); ita != vertices.end(); ita++) {
        itb = ita;
        for (itb++; itb != vertices.end(); itb++) {
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
FaceList *
Polygon::print()
{
    FaceList * list = new FaceList();
    Vector dir(3, 0.0, 0.0, 1.0);

    list->push_back(Face(&vertices, &dir).transform(org, angle));
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
    GLfloat ang_step = PI / POINT_PREC, _ang, end_ang;
    GLfloat vpx, vpy;
    GLdouble tex[2];
    
    /* Sanitizing the angle. */
    while (ang > 360.0f)
        ang -= 360.0f;
    while (ang < 0)
        ang += 360.0f;


    /* Calculating the initial angle. */
    if (st.x != 0)
        _ang = atan(st.y / st.x);
    else if (st.y > 0)
        _ang = PI / 2.0f;
    else
        _ang = 1.5f * PI;
	end_ang = ang * PI / 180.0f + _ang;

    /* Calculating the points and printing them. */
    while (_ang < end_ang) {
        vpx = a * cos(_ang) + cen.x;
        vpy = b * sin(_ang) + cen.y;

        tex[0] = 0.5 * cos(_ang);
        tex[1] = 0.5 * sin(_ang);

        vertices.push_back(new Point(vpx, vpy, cen.z, tex[0], tex[1]));
        _ang += ang_step;
    }

    org[0] = cen.x;
	org[1] = cen.y;
    org[2] = cen.z;

    if (ang == 360.0f)
		mode = GL_POLYGON;
}

/**
 * Prints the ellipse on the screen.
 */
FaceList *
EllArc::print()
{
    FaceList *list = new FaceList();
    Vector dir(3, 0.0, 0.0, 1.0);
    
    list->push_back(Face(&vertices, &dir).transform(org, angle));

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
	return new Point(*vertices.back());
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
		vertices.push_back(
				new Point(
					center.x + rad * cos(idx * ang_step * PI / 180.0f),
				   	center.y + rad * sin(idx * ang_step * PI / 180.0f),
                    center.z,
                    0.5 * cos(idx * ang_step * PI / 180.0f),
                    0.5 * sin(idx * ang_step * PI / 180.0f)
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
	vertices.push_back(new Point(p1.x, p1.y));
	vertices.push_back(new Point(p1.x, p2.y));
	vertices.push_back(new Point(p2.x, p2.y));
	vertices.push_back(new Point(p2.x, p1.y));

    org[0] = (p1.x + p2.x) / 2.0;
    org[1] = (p1.y + p2.y) / 2.0;
    org[2] = (p1.z + p2.z) / 2.0;
}

