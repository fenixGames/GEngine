#include "geometry.h"
#include <math.h>
#include <GL/glut.h>

#define POINT_PREC  180.0f
#define PI  M_PI

using namespace GEngine::Geometry;

/**
 * Constructor of the polyhedra.
 */
Polyhedra::Polyhedra(PointList points)
{
    PointList::iterator iter;

    pointList = new PointList();

    for (iter = points.begin(); iter != points.end(); iter++)
        pointList->push_back(*iter);

    mode = GL_POLYGON;
}

Polyhedra::Polyhedra(Point ** plist, unsigned int npoints)
{
    unsigned int idx;

    /* Check the input. If list is NULL or the number of points is zero, return. */
    if (npoints == 0 || plist == NULL)
        return;

    pointList = new PointList();
    for (idx = 0; idx < npoints; idx++)
        pointList->push_back(plist[idx]);
    
    mode = GL_POLYGON;
}

/**
 * Prints the polyhedra on the screen by interlacing all the points.
 * For more advanced printing, override the function.
 */
PointList *
Polyhedra::print()
{
    PointList::iterator mainIter, toEnd;
    PointList * printing = new PointList();

    /* The way to print is get a vertix and print it with each of the 
     * remaining points.
     */
    for (mainIter = pointList->begin(); mainIter != pointList->end(); mainIter++) {
        toEnd = mainIter;
        for(; toEnd != pointList->end(); toEnd++) {
            printing->push_back(*mainIter);
            printing->push_back(*toEnd);
        }
    }

    return printing;
}

