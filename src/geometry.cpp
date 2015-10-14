#include "geometry.h"
#include <math.h>
#include <GL/glut.h>

#define POINT_PREC  18000.0f
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
Point2D::Point2D(GLint xx, GLint yy)
{
    this->x = xx;
    this->y = yy;
}

/**
 * Prints a 2D point on the screen.
 */
void
Point2D::print()
{
    GLfloat vpx, vpy;

    canvas(this->x, this->y, 0, &vpx, &vpy);
    glBegin(GL_POINTS);
    glVertex2f(vpx, vpy);
    glEnd();
}

/**
 * Overriding the '=' operator for points.
 *
 * @param   Point2D point   The point to be copied.
 * @return The new point created.
 */
Point2D
Point2D::operator = (Point2D point)
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
Point2D::distance(Point2D p1, Point2D p2)
{
    float xcomp, ycomp;

    xcomp = (p1.x - p2.x);
    ycomp = (p1.y - p2.y);

    xcomp *= xcomp;
    ycomp *= ycomp;

    return sqrt(xcomp + ycomp);
}

/**
 * Constructor of the 3D point class.
 * @param   GLint   xx  The horizontal component of the point.
 * @param   GLint   yy  The vertical component of the point.
 * @param   GLint   zz  The depth component of the point.
 */
Point3D::Point3D(GLint xx, GLint yy, GLint zz) : Point2D(xx, yy)
{
    this->z = zz;
}

/* TODO 3D print. */
void
Point3D::print()
{}

/**
 * Contructor of the arc class.
 * @param   Point2D     c   The center of the circunference associated to the arc.
 * @param   Point2D     s   The start point of the arc.
 * @param   GLfloat     a   The angle of the arc.
 */
Arc2D::Arc2D(Point2D c, Point2D s, GLfloat a)
{
    this->center = c;
    this->start = s;
    this->angle = a;

    /* The angle must be defined between 0 and 2 * PI, so we calculate the equivalent one. */
    while (this->angle > 2 * PI + 0.00001)
        this->angle -= 2 * PI;

    while (this->angle < 0)
        this->angle += 2 * PI;
}

/**
 * Prints the arc on the screen, after converting each point, of course.
 */
void
Arc2D::print() {
    GLfloat ang_step = PI / POINT_PREC, ang;
    GLfloat rad = Point2D::distance(this->start, this->center);
    GLfloat vpx, vpy;

    /* Calculating the initial angle. */
    if (this->start.x != 0)
        ang = atan(this->start.y / this->start.x);
    else if (this->start.y > 0)
        ang = PI / 2;
    else
        ang = 3 * PI / 2;

    /* Calculating and printing the required points. */
    //glViewport(this->center.x - rad, this->center.y - rad, 2 * rad, 2 * rad);
    glBegin(GL_LINE_STRIP);
    while (ang < this->angle) {
        vpx = rad * cos(ang) + this->center.x;
        vpy = rad * sin(ang) + this->center.y;

        canvas(vpx, vpy, 0, &vpx, &vpy);
        glVertex2f(vpx, vpy);
        
        ang += ang_step;
    }
    glEnd();
}

/**
 * Retrives the ending point of the arc.
 *
 * @return The ending point of the arc.
 */
Point2D *
Arc2D::getEnd()
{
    GLfloat radius, /* The radius of the circunference of the Arc. */
            sangle; /* The starting angle of the arc. */
    Point2D * end;
    GLint   x, y;

    /* Getting the radius of the arc. */
    radius = Point2D::distance(this->start, this->center);

    /* Getting the initial angle. */
    if (this->start.x != 0)
        sangle = atan(this->start.y / this->start.x);
    else if (this->start.y > 0)
        sangle = PI / 2;
    else
        sangle = 3 * PI / 2;

    /* Getting the coordinates of the new point. */
    x = this->center.x + radius * cos(sangle + this->angle);
    y = this->center.y + radius * sin(sangle + this->angle);

    end = new Point2D(x, y);

    return end;
}

/** 
 * Constructor of the sector, which is an extension of an arc.
 */
Sector2D::Sector2D(Point2D c, Point2D s, GLfloat a) : Arc2D(c, s, a)
{
}

/**
 * Prints the sector on the screen.
 */
void
Sector2D::print()
{
    /* Making the arc for the sector. */
    Arc2D   arc(this->center, this-> start, this->angle);

    /* Getting the end point of the sector. */
    Point2D * endp = arc.getEnd();

    /* Making the two segments. */
    Segment2D s1(this->start, this->center),    /* Start point of the arc -> center of the circunference. */
              s2(this->center, *endp);          /* Center of the circunference -> end point of the arc. */

    /* Print the arc and the two segments. */
    if (!this->solid) {
        arc.print();
        s1.print();
        s2.print();
    } /* TODO Solid color */
}

/**
 * Constructor of the circuference, which is an extension of an arc.
 */
Circunference2D::Circunference2D(Point2D c, GLint radius) : Arc2D(c, Point2D(c.x + radius, c.y), 2 * PI)
{
}

/**
 * Creates a segment which it is a line between two 2D points.
 *
 * @param   Point2D sp  The starting point.
 * @param   Point2D ep  The ending point.
 */
Segment2D::Segment2D(Point2D sp, Point2D ep)
{
    this->start = sp;
    this->end = ep;
}

/**
 * Prints the segment on the screen.
 */
void
Segment2D::print()
{
    GLfloat vpx, vpy;

    glBegin(GL_LINES);
    /* Getting the normalized coordinates of the starting point and printing it. */
    canvas(this->start.x, this->start.y, 0, &vpx, &vpy);
    glVertex2f(vpx, vpy);

    /* Getting the normalized coordinates of the ending point and drawing it. */
    canvas(this->end.x, this->end.y, 0, &vpx, &vpy);
    glVertex2f(vpx, vpy);
    glEnd();
}

/**
 * Creating a polygon object through a list of points.
 *
 * @param   Point2DList list    The list of 2D points that makes the polygon.
 */
Polygon2D::Polygon2D(Point2DList list)
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
Polygon2D::Polygon2D(const Point2D ** list, int number)
{
    int idx;

    for (idx = 0; idx < number; idx++)
        this->pointList.push_back((Point2D *)list[idx]);
}

/**
 * Prints the polygon on the screen.
 */
void
Polygon2D::print()
{
    GLfloat vpx, vpy;
    Point2DList::iterator   iter;
    Point2D * point;

    if (! this->solid) {
        glBegin(GL_LINE_LOOP);

        /* Calculating the normalized points of the polygon in order to print it. */
        for (iter = this->pointList.begin(); iter != this->pointList.end(); iter++) {
            point = *iter;
            canvas(point->x, point->y, 0, &vpx, &vpy);
            glVertex2f(vpx,vpy);
        }
        glEnd();
    }
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
EllArc2D::EllArc2D(Point2D cen, Point2D st, GLfloat a, GLfloat b, GLfloat ang)
{
    this->center = cen;
    this->start = st;
    this->xMod = a;
    this->yMod = b;
    this->angle = ang;

    /* Sanitizing the angle. */
    while (this->angle > 2 * PI + 0.00001)
        this->angle -= 2 * PI;
    while (this->angle < 0)
        this->angle += 2 * PI;
}

/**
 * Prints the ellipse on the screen.
 */
void
EllArc2D::print()
{
    GLfloat ang_step = PI / POINT_PREC, ang;
    GLfloat vpx, vpy;

    /* Calculating the initial angle. */
    if (this->start.x != 0)
        ang = atan(this->start.y / this->start.x);
    else if (this->start.y > 0)
        ang = PI / 2.0f;
    else
        ang = 1.5f * PI;

    glBegin(GL_LINE_STRIP);
    /* Calculating the points and printing them. */
    while (ang < this->angle) {
        vpx = this->xMod * cos(ang) + this->center.x;
        vpy = this->yMod * sin(ang) + this->center.y;

        canvas(vpx, vpy, 0, &vpx, &vpy);
        glVertex2f(vpx, vpy);
        ang += ang_step;
    }
    glEnd();
}

