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
    theDisplay->camera->activate();
    theDisplay->scene->print();
    theDisplay->camera->deactivate();

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

    /* Use orthographic view until we build the world class and define where the camera will be. */
/*    glFrustum(- 0.5 * screen[0], 0.5 * screen[0], -0.5 * screen[1], 
            0.5 * screen[1], 0.5 * screen[2], 1.0 * screen[2]);
*/
    glOrtho(- 0.5 * screen[0], 0.5 * screen[0], -0.5 * screen[1], 
            0.5 * screen[1], 0.001 * screen[2], 1.0 * screen[2]);

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

