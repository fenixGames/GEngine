/**
 * This file contains all the functions used by the display related to the 2D figures
 * and related to the 2D axis, such as rotation and translation.
 *
 * @author  Roberto Fernandez Cueto
 * @date    28.10.2015
 *
 * $Id$
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
 * @param Camera        * cam   The camera which records the scene.
 * @param GLuint        depth   The depth of the scene.
 *
 * @return Returns 0 on success or the error code on failure.
 */
int 
printFigures(FigureList * list, int winId, Camera * cam, GLuint depth)
{
    FaceList                * faces;
    FaceList::iterator      faceIt;
    PointList::iterator     pointIter;
    FigureList::iterator    iter;
	double					dist;
    Vector                  camDir = cam->getDirection();

    /* If the list is NULL we should draw nothing. */
    if (list == NULL)
        return 0;

    /* If there is no camera, nothing must be shown. */
    if (cam == NULL)
        return 0;

#ifdef DEBUG
    StaticCamera axiscam(*cam);

    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    axiscam.move(Point());

    axiscam.activate();
	glBegin(GL_LINES);
    glColor3d( 1.0, 0.0, 0.0);
    glVertex3d(-1.0, 0.0, 0.0);
    glVertex3d( 1.0, 0.0, 0.0);
    
    glColor3d(0.0, 1.0, 0.0);
    glVertex3d(0.0, -1.0, 0.0);
    glVertex3d(0.0,  1.0, 0.0);
    
    glColor3d(0.0, 0.0, 1.0);
    glVertex3d(0.0, 0.0, -1.0);
    glVertex3d(0.0, 0.0, 1.0);
    glEnd();

    axiscam.deactivate();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
#endif

    cam->activate();
    /* Going through the list of figures. */
    for (iter = list->begin(); iter != list->end(); iter++) {
        faces = (*iter)->print();
        
        (*iter)->activeMaterial();

        /* Going through each of the faces. */
        for (faceIt = faces->begin(); faceIt != faces->end(); faceIt++) {
            glBegin((*iter)->getMode());

            /* Getting if the face is front or back. */
            if (camDir * (*(*faceIt)->normal) > 0.0) /* FRONT */ {
                (*faceIt)->vertex->reverse();
            }

            /* Printing the points. */
            for (pointIter = (*faceIt)->vertex->begin(); pointIter != (*faceIt)->vertex->end(); pointIter++){
                dist = depth - cam->getDistance(Point(*(*pointIter)));
                if (dist == 0.0)
                    dist = 1.0e-6;

    			/* Printing the points. */
                glTexCoord2d((*pointIter)->s, (*pointIter)->t);
//                glVertex3d((*pointIter)->x, (*pointIter)->y, (*pointIter)->z);
    			glVertex4d((*pointIter)->x, (*pointIter)->y, (*pointIter)->z, depth / dist);
            }

            glEnd();
        }
        delete faces;
        (*iter)->deactivateMaterial();
    }

	cam->deactivate();
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

    /* Initializing the camera to NULL. */
    camera = NULL;

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
    /* Cleans the screen. */
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glViewport(theDisplay->position[0], theDisplay->position[1],
            theDisplay->screen[0], theDisplay->screen[1]);
    glColor3f(theDisplay->fgcolor[0], theDisplay->fgcolor[1], theDisplay->fgcolor[2]);

    /* Prints the figures of the list. */
    printFigures(theDisplay->figures, theDisplay->mainWin, theDisplay->camera, theDisplay->screen[2]);

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

/**
 * Initialize the GL functions before printing.
 */
void
Display::initGL()
{
    /* Enabling Lighting, textures and depth. */
    glEnable(GL_DEPTH);
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_LINE_SMOOTH);
    glShadeModel(GL_SMOOTH);
//    glEnable(GL_LIGHTING);
    
    /* Setting the background color. */
    glClearColor(bgcolor[0], bgcolor[1], bgcolor[2], 0.0f);

    /* Setting the initial modelview and projection matrices. */
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(- 0.5 * screen[0], 0.5 * screen[0], -0.5 * screen[1], 
            0.5 * screen[1], -0.5 * screen[2], 0.5 * screen[2]);

    /* Setting the sizes of the points and the lines if needed. */
//    glPointSize(4.0f);
    
}

/**
 * Attaches a camera to the display.
 * If a previous one was attached, destroy it.
 * @param   Camera  * cam   The camera to be attached.
 */
void
Display::attachCamera(Camera    *cam)
{
    camera = cam;
}

/**
 * Creates a new camera and attaches it to the display.
 * Destroys the camera already attached to the display.
 * @param   Point   pos     The position of the camera.
 * @param   double  yaw     The yaw of the camera.
 * @param   double  pitch   The pitch of the camera.
 * @param   double  roll    The roll of the camera.
 */
void
Display::newCamera(Point pos, double yaw, double pitch, double roll)
{
    Camera  * cam = new StaticCamera(pos, yaw, pitch, roll);
    attachCamera(cam);
}

