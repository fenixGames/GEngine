/**
 * This file contains all the functions used by the display related to the 3D figures
 * and related to the 3D axis, such as rotation and translation.
 *
 * @author  Roberto Fernandez Cueto
 * @date    28.10.2015
 */
#include "display.h"
#include "geometry.h"
#include <stdio.h>

using namespace GEngine;
using namespace D3D;

/** 
 * The function used to print 3D figures on the screen.
 *
 * @param Figure3DList  * list  The list of figures to draw.
 * @param int           winId   The id of the window where to draw the points.
 *
 * @return Returns 0 on success or the error code on failure.
 */
int 
print3D(Figure3DList * list, int winId, Matrix *trans)
{
    Point3DList             * pointList;
    Point3DList::iterator   pointIter;
    Figure3DList::iterator  iter;
	Vector				 	out(4, 0, 0, 0, 0);
	double					vpx, vpy, vpz;

    /* If the list is NULL we should draw nothing. */
    if (list == NULL)
        return 0;

    /* Going through the list of figures. */
    for (iter = list->begin(); iter != list->end(); iter++) {
        pointList = (*iter)->print();
		glColor3d((*iter)->getRed(), (*iter)->getGreen(), (*iter)->getBlue());
		glBegin((*iter)->getMode());
        for (pointIter = pointList->begin(); pointIter != pointList->end(); pointIter++) {
            /* Convert point, translate it and rotate it if needed in absolute coordinates. */
			vpx = (*pointIter)->x;
			vpy = (*pointIter)->y;
            vpz = (*pointIter)->z;

			out = (* trans) * Vector(4, vpx, vpy, vpz, 1.0);

			/* Printing the points. */
			glVertex3d(out.getElement(0), out.getElement(1), out.getElement(2));
        }
		glEnd();
    }
	delete pointList;
	return 0;
}

/**
 * Translates all the displayable figures to the given point. Usefull to have a different origin of coordinates.
 *
 * @param   Point   point   The new origin of coordinates.
 */
void
Display::translate3D(Point  *point)
{
    double  x11, x12, y11, y12, z11, z12;
    x11 = (2.0 * point->x) / (screen[0] - 1.0);
    x12 = point->x;
    y11 = (2.0 * point->y) / (screen[1] - 1.0);
    y12 = point->y;
    z11 = (2.0 * point->z) / (screen[2] - 1.0);
    z12 = point->z;
    Matrix  translate(4, 4,
            x11, 0.0, 0.0, x12,
            0.0, y11, 0.0, y12,
            0.0, 0.0, z11, z12, /* Leave the Z axis the same. */
            0.0, 0.0, 0.0, 1.0);/* The constant component should be left the same. */

    * trans = (*trans) * translate;
}

/**
 * Adds a figure object to the list of objects to be printed.
 * @param Figure    *figure     The figure to be added to the list.
 */
void
Display::add3DFigure(Figure * figure)
{
	/* Setting the list of figures. */
	if (figures3D == NULL )
		figures3D = new Figure3DList();

	/* Adding the figure. */
	if (figures3D != NULL)
	    figures3D->push_back(figure);
}

/**
 * Removes the figure from the list of figures to print.
 * @param Figure    *figure     The figure to be removed.
 */
void
Display::remove3DFigure(Figure *figure)
{
	/* Removing the figure. */
	if (figures3D != NULL)
	    figures3D->remove(figure);
}

/**
 * Retrives the last item of the figure list and removes it.
 * @return The requested item.
 */
Figure *
Display::pop3DFigure()
{
	/* If the list has not been set return NULL. */
	if (figures3D == NULL)
		return NULL;

    Figure * item = figures3D->back();
    figures3D->pop_back();

    return item;
}

/**
 * Retrieves the first element of the list and removes it.
 * @return The requested element.
 */
Figure *
Display::shift3DFigure()
{
	/* If the list has not been set return NULL. */
	if (figures3D == NULL)
		return NULL;

    Figure * item = figures3D->front();
    figures3D->pop_front();

    return item;
}

/**
 * Retrieves the first item of the list and returns it.
 * @return The item requested.
 */
Figure *
Display::first3DFigure()
{
	/* If the list has not been set return NULL. */
	if (figures3D == NULL)
		return NULL;

    return figures3D->front();
}

/**
 * Retrieves the last item of the list and returns it.
 * @return The item requested.
 */
Figure *
Display::last3DFigure()
{
	/* If the list has not been set return NULL. */
	if (figures3D == NULL)
		return NULL;

    return figures3D->back();
}

