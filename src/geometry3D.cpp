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
Point::Point(GLint xx, GLint yy, GLint zz) : D2D::Point(xx, yy)
{
    this->z = zz;
}

/* TODO 3D print. */
void
Point::print()
{}


