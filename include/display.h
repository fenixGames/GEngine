/**
 * This file contains the information needed to manage the display object, which is in charge of drawing
 * the windows and printing the lines, objects and everything on the screen.
 *
 * @author  Roberto Fernandez Cueto
 * @date    14.10.2015
 *
 * $Id: $
 */
#ifndef _DISPLAY_H
#define _DISPLAY_H

#include <GL/gl.h>
#include "geometry.h"
#include "matrix.h"

#ifndef _GENGINE_H
namespace GEngine {
    class Display;
    struct camera;
}
#endif

#define FigureList      std::list<GEngine::Geometry::Figure *>

/**
 * The structure to define how the camera should be implemented.
 */
struct GEngine::camera {
    GLfloat yaw;            /* The angle to control the yaw of the camera. */
    GLfloat pitch;          /* The angle to control the pitch of the camera. */
    GLfloat roll;           /* The angle to control the roll of the camera. */
    GLint  position[3];    /* The position of the camera. */
    GLuint  screen[3];      /* The size of the projected screen. */
};

/**
 * The class associated to the display engine whose in charge of printing everything on the screen.
 * The user should think in the display as the camera of the Engine.
 */
class GEngine::Display {
	private:
        static void displayInit();
        static void SwapBuffers();
        void initGL();
    protected:
        GLuint screen[3];   /* Dimensions of the main window. */
        GLuint position[2]; /* Position of the main window. */
        GLfloat bgcolor[3]; /* The color of the background. */
        GLfloat fgcolor[3]; /* The color of the foreground. */
        char    *title;     /* The title of the window associated to this display. */

        static Display  *theDisplay; /* The main screen display. */
        int mainWin;    /* The identifier of the main Window. */

        FigureList      * figures;  /* The list of figures of the display. */

        struct camera   displayCam; /* The camera of the display. */

        /* The function to draw the screen. */
        static void displayFunc();
    public:
        Display(GLuint width, GLuint height, GLuint depth = 0, GLuint x = 0, GLuint y = 0);
		~Display();

        /* Prints everything on the screen. */
        int print();

        /* Sets the background color. */
        void bkgColor(unsigned char red, unsigned char green, unsigned char blue);

        /* Sets the foreground color. */
        void frgColor(unsigned char red, unsigned char green, unsigned char blue);

        /* Sets the title of the actual window. */
        bool setTitle(const char * title);

		/* Rotates the whole display. */
		void roll(GLfloat angle);
        void yaw(GLfloat angle);
        void pitch(GLfloat angle);

        /* Translates the whole display in a 2D or 3D environment. */
        void translate(Geometry::Point point);

        /* Adds a figure to be drawn to the list of objects to draw. */
        void addFigure(Geometry::Figure * figure);

        /* Removes the figure of the list. */
        void removeFigure(Geometry::Figure * figure);

        /* Retrives the figure from the back of the list, removing it. */
        Geometry::Figure * popFigure();

        /* Retrives the figure from the beginning of the list, removing it. */
        Geometry::Figure * shiftFigure();

        /* Retrives the figure from the front of the list. Does not remove it. */
        Geometry::Figure * firstFigure();

        /* Retrieves the last figure from the list. It does not remove it. */
        Geometry::Figure * lastFigure();
};

#endif
