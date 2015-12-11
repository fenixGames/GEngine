/**
 * This file contains the information needed to manage the display object, which is in charge of drawing
 * the windows and printing the lines, objects and everything on the screen.
 *
 * @author  Roberto Fernandez Cueto
 * @date    14.10.2015
 *
 * $Id$
 */
#ifndef _DISPLAY_H_
#define _DISPLAY_H_

#include <GL/gl.h>
#include "world.h"

namespace GEngine {
    class Display;
}

/**
 * The class associated to the display engine whose in charge of printing everything on the screen.
 * The user should think in the display as the camera of the Engine.
 */
class GEngine::Display {
	private:
        static void displayInit();
        static void SwapBuffers();
        static void idleRender();
        void initGL();
    protected:
        GLuint screen[3];   /* Dimensions of the main window. */
        GLuint position[2]; /* Position of the main window. */
        GLfloat bgcolor[3]; /* The color of the background. */
        GLfloat fgcolor[3]; /* The color of the foreground. */
        char    *title;     /* The title of the window associated to this display. */

        static Display  *theDisplay; /* The main screen display. */
        int mainWin;    /* The identifier of the main Window. */

        Scene * scene;
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

        /* Sets the current scene to display. */
        void setScene(Scene * scene);
};

#endif
