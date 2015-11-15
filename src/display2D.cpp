/**
 * This file contains all the functions used by the display related to the 2D figures
 * and related to the 2D axis, such as rotation and translation.
 *
 * @author  Roberto Fernandez Cueto
 * @date    28.10.2015
 */
#include "display.h"
#include "geometry.h"
#include <stdio.h>

using namespace GEngine;
using namespace D2D;

/** 
 * The function used to print 2D figures on the screen.
 *
 * @param Figure2DList  * list  The list of figures to draw.
 * @param int           winId   The id of the window where to draw the points.
 *
 * @return Returns 0 on success or the error code on failure.
 */
int 
print2D(Figure2DList * list, int winId, Matrix *trans)
{
    Point2DList             * pointList;
    Point2DList::iterator   pointIter;
    Figure2DList::iterator  iter;
	Vector				 	out(3, 0, 0, 0);
	double					vpx, vpy;

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
			out = (* trans) * Vector(3, vpx, vpy, 1.0);

			/* Printing the points. */
			glVertex2d(out.getElement(0), out.getElement(1));
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
Display::translate2D(Point  *point)
{
    double  x11, x12, y11, y12;
    x11 = (2.0 * point->x) / (screen[0] - 1.0);
    x12 = point->x;
    y11 = (2.0 * point->y) / (screen[1] - 1.0);
    y12 = point->y;
    Matrix  translate(3, 3, x11, 0.0, x12, 0.0, y11, y12, 0.0, 0.0, 1.0);

    * trans = (*trans) * translate;
}

/**
 * Adds a figure object to the list of objects to be printed.
 * @param Figure    *figure     The figure to be added to the list.
 */
void
Display::add2DFigure(Figure * figure)
{
	/* Setting the list of figures. */
	if (figures2D == NULL )
		figures2D = new Figure2DList();

	/* Adding the figure. */
	if (figures2D != NULL)
	    figures2D->push_back(figure);
}

/**
 * Removes the figure from the list of figures to print.
 * @param Figure    *figure     The figure to be removed.
 */
void
Display::remove2DFigure(Figure *figure)
{
	/* Removing the figure. */
	if (figures2D != NULL)
	    figures2D->remove(figure);
}

/**
 * Retrives the last item of the figure list and removes it.
 * @return The requested item.
 */
Figure *
Display::pop2DFigure()
{
	/* If the list has not been set return NULL. */
	if (figures2D == NULL)
		return NULL;

    Figure * item = figures2D->back();
    figures2D->pop_back();

    return item;
}

/**
 * Retrieves the first element of the list and removes it.
 * @return The requested element.
 */
Figure *
Display::shift2DFigure()
{
	/* If the list has not been set return NULL. */
	if (figures2D == NULL)
		return NULL;

    Figure * item = figures2D->front();
    figures2D->pop_front();

    return item;
}

/**
 * Retrieves the first item of the list and returns it.
 * @return The item requested.
 */
Figure *
Display::first2DFigure()
{
	/* If the list has not been set return NULL. */
	if (figures2D == NULL)
		return NULL;

    return figures2D->front();
}

/**
 * Retrieves the last item of the list and returns it.
 * @return The item requested.
 */
Figure *
Display::last2DFigure()
{
	/* If the list has not been set return NULL. */
	if (figures2D == NULL)
		return NULL;

    return figures2D->back();
}

