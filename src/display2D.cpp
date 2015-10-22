#include "display.h"
#include "geometry.h"

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
static int 
print2D(Figure2DList * list, int winId)
{
    Point2DList             * pointList;
    Point2DList::iterator   pointIter;
    Figure2DList::iterator  iter;

    /* If the list is NULL we should draw nothing. */
    if (list == NULL)
        return 0;

    /* Going through the list of figures. */
    for (iter = list->begin(); iter != list->end(); iter++) {
        pointList = (*iter)->print();
        for (pointIter = pointList->begin(); pointIter != pointList->end(); pointIter++) {
            /* TODO Convert point, translate it and rotate it if needed. */
        }
    }
}

/**
 * Adds a figure object to the list of objects to be printed.
 * @param Figure    *figure     The figure to be added to the list.
 */
void
Display::add2DFigure(Figure * figure)
{
    this->figures2D->push_back(figure);
}

/**
 * Removes the figure from the list of figures to print.
 * @param Figure    *figure     The figure to be removed.
 */
void
Display::remove2DFigure(Figure *figure)
{
    this->figures2D->remove(figure);
}

/**
 * Retrives the last item of the figure list and removes it.
 * @return The requested item.
 */
Figure *
Display::pop2DFigure()
{
    Figure * item = this->figures2D->back();
    this->figures2D->pop_back();

    return item;
}

/**
 * Retrieves the first element of the list and removes it.
 * @return The requested element.
 */
Figure *
Display::shift2DFigure()
{
    Figure * item = this->figures2D->front();
    this->figures2D->pop_front();

    return item;
}

/**
 * Retrieves the first item of the list and returns it.
 * @return The item requested.
 */
Figure *
Display::first2DFigure()
{
    return this->figures2D->front();
}

/**
 * Retrieves the last item of the list and returns it.
 * @return The item requested.
 */
Figure *
Display::last2DFigure()
{
    return this->figures2D->back();
}
