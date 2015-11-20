/**
 * This file contains all the functions used by the display related to the 2D figures
 * and related to the 2D axis, such as rotation and translation.
 *
 * @author  Roberto Fernandez Cueto
 * @date    28.10.2015
 */
#include "display.h"
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>

using namespace GEngine;
using namespace GEngine::Geometry;

/** 
 * The function used to print figures on the screen.
 *
 * @param FigureList    * list  The list of figures to draw.
 * @param int           winId   The id of the window where to draw the points.
 *
 * @return Returns 0 on success or the error code on failure.
 */
int 
printFigures(FigureList * list, int winId, struct camera cam)
{
    PointList               * pointList;
    PointList::iterator     pointIter;
    FigureList::iterator    iter;
    Matrix                  tr, rotx, roty, rotz, proj, transf, total;
#ifdef DEBUG
    Matrix                  axis(4, 6, 
                                -1.0, 1.0,  0.0, 0.0,  0.0, 0.0,
                                 0.0, 0.0, -1.0, 1.0,  0.0, 0.0,
                                 0.0, 0.0,  0.0, 0.0, -1.0, 1.0,
                                 0.0, 0.0,  0.0, 0.0,  0.0, 0.0), result;
#endif
	Vector				 	out(4, 0, 0, 0, 0);
	double					vpx, vpy, vpz, dist;

    /* If the list is NULL we should draw nothing. */
    if (list == NULL)
        return 0;

    /* Create the translation and rotation matrices. */
    tr = Matrix(4, 4, 
            1.0, 0.0, 0.0, (double) - cam.position[0],
            0.0, 1.0, 0.0, (double) - cam.position[1],
            0.0, 0.0, 1.0, (double) - cam.position[2],
            0.0, 0.0, 0.0, 1.0);
    rotx = Matrix(4, 4,
            1.0, 0.0,           0.0,            0.0,
            0.0, cos(cam.yaw),  -sin(cam.yaw),  0.0,
            0.0, sin(cam.yaw),  cos(cam.yaw),   0.0,
            0.0, 0.0,           0.0,            1.0);
    roty = Matrix(4, 4,
            cos(cam.pitch), 0.0, -sin(cam.pitch),   0.0,
            0.0,            1.0, 0.0,               0.0,
            sin(cam.pitch), 0.0, cos(cam.pitch),    0.0,
            0.0,            0.0, 0.0,               1.0);
    rotz = Matrix(4, 4,
            cos(cam.roll),  -sin(cam.roll), 0.0,    0.0,
            sin(cam.roll),  cos(cam.roll),  0.0,    0.0,
            0.0,            0.0,            1.0,    0.0,
            0.0,            0.0,            0.0,    1.0);
    vpx = 2.0 / (cam.screen[0] - 1.0);
    vpy = 2.0 / (cam.screen[1] - 1.0);
    vpz = 2.0 / (cam.screen[2] - 1.0);
    transf = Matrix(4, 4,
            vpx,    0.0,    0.0,    0.0,
            0.0,    vpy,    0.0,    0.0,
            0.0,    0.0,    vpz,    0.0,
            0.0,    0.0,    0.0,    1.0);

    total = transf * rotx * roty * rotz * tr;
#ifdef DEBUG
    result = rotx * roty * rotz * axis;
/*    result.print();
    tr.print();
    total.print();*/

    glViewport( 5, cam.screen[1] - 55, 50, 50 );
    glBegin(GL_LINES);
    glColor3d( 1.0, 0.0, 0.0);
    glVertex3d(result.getElement(0, 0), result.getElement(1, 0), result.getElement(2, 0));
    glVertex3d(result.getElement(0, 1), result.getElement(1, 1), result.getElement(2, 1));
    
    glColor3d(0.0, 1.0, 0.0);
    glVertex3d(result.getElement(0, 2), result.getElement(1, 2), result.getElement(2, 2));
    glVertex3d(result.getElement(0, 3), result.getElement(1, 3), result.getElement(2, 3));
    
    glColor3d(0.0, 0.0, 1.0);
    glVertex3d(result.getElement(0, 4), result.getElement(1, 4), result.getElement(2, 4));
    glVertex3d(result.getElement(0, 5), result.getElement(1, 5), result.getElement(2, 5));
    glEnd();
    glViewport( 0, 0, cam.screen[0], cam.screen[1] );
#endif
    /* Going through the list of figures. */
    for (iter = list->begin(); iter != list->end(); iter++) {
        pointList = (*iter)->print();
        if ((*iter)->getSolid())
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        else
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

		glColor3d((*iter)->getRed(), (*iter)->getGreen(), (*iter)->getBlue());
		glBegin((*iter)->getMode());
        for (pointIter = pointList->begin(); pointIter != pointList->end(); pointIter++) {
            /* Convert point, translate it and rotate it if needed in absolute coordinates. */
			vpx = (*pointIter)->x;
			vpy = (*pointIter)->y;
            vpz = (*pointIter)->z;

			out = total * Vector(4, vpx, vpy, vpz, 1.0);

            /* Getting the distance to the camera. */
            vpx = (double) out.getElement(0);
            vpy = (double) out.getElement(1);
            vpz = (double) out.getElement(2);
            dist = sqrt(vpx * vpx + vpy * vpy + vpz * vpz);

			/* Printing the points. */
			glVertex4d(out.getElement(0), out.getElement(1), out.getElement(2), dist);
        }
		glEnd();
    }
	delete pointList;
	return 0;
}

/**
 * Inititializes the display class and the main window's size and position.
 *
 * @param GLuint    width   The width of the main window for the application.
 * @param GLuint    height  The height of the main window for the application.
 * @param GLuint    depth   The depth of the main window for the application.
 * @param GLuint    x       The horizontal position of the window on the screen.
 * @param GLuint    y       The vertical position of the window on the screen.
 */
Display::Display(GLuint width, GLuint height, GLuint depth, GLuint x, GLuint y)
{
	double ax, ay, az;

    /* Setting the components of this class. */
    screen[0] = width;
    screen[1] = height;
    screen[2] = depth;

    position[0] = x;
    position[1] = y;

    bgcolor[0] = 1.0f;
    bgcolor[1] = 1.0f;
    bgcolor[2] = 1.0f;

    fgcolor[0] = 0.0f;
    fgcolor[1] = 0.0f;
    fgcolor[2] = 0.0f;

    /* Initializing the title of the window to NULL. */
    title = NULL;

    /* Initializing the lists of figures to NULL. */
    figures = NULL;

	/* Settign the transformation matrix to the default values. */
	ax = 2.0 / (screen[0] - 1.0); /* Setting the X modifier. */
	ay = 2.0 / (screen[1] - 1.0); /* Setting the Y modifier. */
    az = (screen[2] > 0 ? 2.0 / (screen[2] - 1.0) : 0); /* Setting the Z modifier. */
    memset(&displayCam, 0, sizeof(struct camera));
    memcpy(&displayCam.screen, screen, sizeof(GLuint) * 3);

	if (theDisplay == NULL) {
		/* OS initialization. */
        displayInit();

		theDisplay = this;
	}
}

/**
 * Destructor of the display.
 */
Display::~Display()
{
	if (title != NULL)
		free(title);
	if (figures != NULL)
		delete figures;
}

/**
 * Draws everything on the screen.
 */
void
Display::displayFunc()
{
    FigureList *list = NULL;    /* Shortcut for Display::theDisplay->figures2D */

    /* Getting the figure list. */
    if (Display::theDisplay != NULL && Display::theDisplay->figures != NULL) {
        list = Display::theDisplay->figures;
    } 

    /* Cleans the screen. */
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glViewport(Display::theDisplay->position[0], Display::theDisplay->position[1],
            Display::theDisplay->screen[0], Display::theDisplay->screen[1]);
    glColor3f(Display::theDisplay->fgcolor[0],
            Display::theDisplay->fgcolor[1], Display::theDisplay->fgcolor[2]);

    /* Setting the Basic axis to debug the engine's printing. */
    glPointSize(4.0f);
    
    /* Prints the figures of the list. */
    printFigures(list, Display::theDisplay->mainWin, Display::theDisplay->displayCam);

    /* Swaps the buffers so the printing will be visible. */
    SwapBuffers();
}

/**
 * Sets the background color.
 *
 * @param unsigned char r   The red component of the color.
 * @param unsigned char g   The green component of the color.
 * @param unsigned char b   The blue component of the color.
 */
void
Display::bkgColor(unsigned char r, unsigned char g, unsigned char b)
{
    bgcolor[0] = (GLfloat)(r / 255);
    bgcolor[1] = (GLfloat)(g / 255);
    bgcolor[2] = (GLfloat)(b / 255);
}

/**
 * Sets the background color.
 *
 * @param unsigned char r   The red component of the color.
 * @param unsigned char g   The green component of the color.
 * @param unsigned char b   The blue component of the color.
 */
void
Display::frgColor(unsigned char r, unsigned char g, unsigned char b)
{
    fgcolor[0] = (GLfloat)(r / 255);
    fgcolor[1] = (GLfloat)(g / 255);
    fgcolor[2] = (GLfloat)(b / 255);
}


/**
 * Sets the title to the window associated to this display.
 * @return Returns whether the title could be set or not.
 */
bool
Display::setTitle(const char * _title)
{
    unsigned    length = strlen(_title);

    /* Checking whether the length of the title is too big. */
    if (length + 1 < length)
        return false;

    /* Allocating memory for the title. */
    title = (char *) malloc((++length) * sizeof(char));
    
    /* Copying the title. */
    if (title != NULL) {
        strncpy(title, _title, length);
    }

    return title != NULL;
}

/**
 * Rotate the whole display the angle indicated in the argument using the Z axis
 * as the central axis for rotation.
 *
 * @param	GLfloat	angle	The angle to rotate the display.
 */
void
Display::roll(GLfloat angle) {
	/* Normalizing the angle (0 <= angle <= 360). */
	while (angle > 360.0f)
		angle -= 360.0f;

	while (angle < 0.0f)
		angle += 360.0f;

	angle *= M_PI / 180.0f;

    /* Setting the angle. */
    displayCam.roll = angle;
}

/**
 * Rotate the whole display the angle indicated in the argument using the X axis
 * as the central axis for rotation.
 *
 * @param	GLfloat	angle	The angle to rotate the display.
 */
void
Display::yaw(GLfloat angle) {
	/* Normalizing the angle (0 <= angle <= 360). */
	while (angle > 360.0f)
		angle -= 360.0f;

	while (angle < 0.0f)
		angle += 360.0f;

	angle *= M_PI / 180.0f;

    /* Setting the angle. */
    displayCam.yaw = angle;
}

/**
 * Rotate the whole display the angle indicated in the argument using the Y axis
 * as the central axis for rotation.
 *
 * @param	GLfloat	angle	The angle to rotate the display.
 */
void
Display::pitch(GLfloat angle) {
	/* Normalizing the angle (0 <= angle <= 360). */
	while (angle > 360.0f)
		angle -= 360.0f;

	while (angle < 0.0f)
		angle += 360.0f;

	angle *= M_PI / 180.0f;

    /* Setting the angle. */
    displayCam.pitch = angle;
}

/**
 * Translates all the displayable figures to the given point. Usefull to have a different origin of coordinates.
 *
 * @param   Point   point   The new origin of coordinates.
 */
void
Display::translate(Point point)
{
    displayCam.position[0] = (GLint) point.x;
    displayCam.position[1] = (GLint) point.y;
    displayCam.position[2] = (GLint) point.z;
}

/**
 * Adds a figure object to the list of objects to be printed.
 * @param Figure    *figure     The figure to be added to the list.
 */
void
Display::addFigure(Figure * figure)
{
	/* Setting the list of figures. */
	if (figures == NULL )
		figures = new FigureList();

	/* Adding the figure. */
	if (figures != NULL)
	    figures->push_back(figure);
}

/**
 * Removes the figure from the list of figures to print.
 * @param Figure    *figure     The figure to be removed.
 */
void
Display::removeFigure(Figure *figure)
{
	/* Removing the figure. */
	if (figures != NULL)
	    figures->remove(figure);
}

/**
 * Retrives the last item of the figure list and removes it.
 * @return The requested item.
 */
Figure *
Display::popFigure()
{
	/* If the list has not been set return NULL. */
	if (figures == NULL)
		return NULL;

    Figure * item = figures->back();
    figures->pop_back();

    return item;
}

/**
 * Retrieves the first element of the list and removes it.
 * @return The requested element.
 */
Figure *
Display::shiftFigure()
{
	/* If the list has not been set return NULL. */
	if (figures == NULL)
		return NULL;

    Figure * item = figures->front();
    figures->pop_front();

    return item;
}

/**
 * Retrieves the first item of the list and returns it.
 * @return The item requested.
 */
Figure *
Display::firstFigure()
{
	/* If the list has not been set return NULL. */
	if (figures == NULL)
		return NULL;

    return figures->front();
}

/**
 * Retrieves the last item of the list and returns it.
 * @return The item requested.
 */
Figure *
Display::lastFigure()
{
	/* If the list has not been set return NULL. */
	if (figures == NULL)
		return NULL;

    return figures->back();
}

