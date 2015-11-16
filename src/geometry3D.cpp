#include "geometry.h"
#include <math.h>
#include <GL/glut.h>

#define POINT_PREC  180.0f
#define PI  M_PI

using namespace D3D;

/**
 * Constructor of the 3D point class.
 * @param   GLint   xx  The horizontal component of the point.
 * @param   GLint   yy  The vertical component of the point.
 * @param   GLint   zz  The depth component of the point.
 */
Point::Point(GLint xx, GLint yy, GLint zz)
{
    x = xx;
    y = yy;
    z = zz;
}

/**
 */
Point3DList * 
Point::print()
{
    Point3DList * list = new Point3DList();

    list->push_back(new Point(x, y, z));

    return list;
}


