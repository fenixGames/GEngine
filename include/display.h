/**
 * This file contains the information needed to manage the display object, which is in charge of drawing
 * the windows and printing the lines, objects and everything on the screen.
 *
 * @author  Roberto Fernandez Cueto
 * @date    14.10.2015
 */
#ifndef _DISPLAY_H
#define _DISPLAY_H

#include <GL/gl.h>
#include <GL/glut.h>
#include "geometry.h"

#define FigureList  std::list<Figure *>

/**
 * The class associated to the display engine whose in charge of printing everything on the screen.
 */
class Display {
    protected:
        GLuint screen[2];   /* Dimensions of the main window. */
        GLuint position[2]; /* Position of the main window. */
        GLfloat bgcolor[3]; /* The color of the background. */
        GLfloat fgcolor[3]; /* The color of the foreground. */
        char    *title;     /* The title of the window associated to this display. */

        static Display *theDisplay; /* The main screen display. */
        int mainWin;    /* The identifier of the main Window. */

        FigureList  figures;
        /* The function to draw the screen. */
        static void displayFunc();
    public:
//        Display(int *argc, char **argv, GLuint width, GLuint height, GLuint x = 0, GLuint y = 0);
        Display(GLuint width, GLuint height, GLuint x = 0, GLuint y = 0);

        /* Prints everything on the screen. */
        int print();

        /* Sets the background color. */
        void bkgColor(unsigned char red, unsigned char green, unsigned char blue);

        /* Sets the foreground color. */
        void frgColor(unsigned char red, unsigned char green, unsigned char blue);

        /* Sets the title of the actual window. */
        bool setTitle(const char * title);

        /* Adds a figure to be drawn to the list of objects to draw. */
        void addFigure(Figure * figure);

        /* Removes the figure of the list. */
        void removeFigure(Figure * figure);

        /* Retrives the figure from the back of the list, removing it. */
        Figure * popFigure();

        /* Retrives the figure from the beginning of the list, removing it. */
        Figure * shiftFigure();

        /* Retrives the figure from the front of the list. Does not remove it. */
        Figure * firstFigure();

        /* Retrieves the last figure from the list. It does not remove it. */
        Figure * lastFigure();
};

#endif
