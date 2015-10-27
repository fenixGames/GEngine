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
#include "matrix.h"

#ifndef _GENGINE_H
namespace GEngine {
    class Display;
}
#endif

#define Figure2DList    std::list<D2D::Figure *>
#define Figure3DList    std::list<D3D::Figure *>

/**
 * The class associated to the display engine whose in charge of printing everything on the screen.
 */
class GEngine::Display {
	private:
    protected:
        GLuint screen[2];   /* Dimensions of the main window. */
        GLuint position[2]; /* Position of the main window. */
        GLfloat bgcolor[3]; /* The color of the background. */
        GLfloat fgcolor[3]; /* The color of the foreground. */
        char    *title;     /* The title of the window associated to this display. */

        static Display *theDisplay; /* The main screen display. */
        int mainWin;    /* The identifier of the main Window. */

        Figure2DList    * figures2D;
//        Figure3DList    * figures3D; TODO
//
        /* Transformation matrix. */
		Matrix	* trans;

        /* The function to draw the screen. */
        static void displayFunc();
    public:
        Display(GLuint width, GLuint height, GLuint x = 0, GLuint y = 0);

        /* Prints everything on the screen. */
        int print();

        /* Sets the background color. */
        void bkgColor(unsigned char red, unsigned char green, unsigned char blue);

        /* Sets the foreground color. */
        void frgColor(unsigned char red, unsigned char green, unsigned char blue);

        /* Sets the title of the actual window. */
        bool setTitle(const char * title);

		/* Rotates the whole display. */
		void rotate(GLfloat angle);

        /* Translates the whole display in a 2D or 3D environment. */
        void translate2D(D2D::Point  * point);
//        void translate3D(D3D::Point * point); TODO

		/* Bends the display. TODO 3D */
		/*void bend(GLfloat angle);*/

        /* Adds a figure to be drawn to the list of objects to draw. */
        void add2DFigure(D2D::Figure * figure);

        /* Removes the figure of the list. */
        void remove2DFigure(D2D::Figure * figure);

        /* Retrives the figure from the back of the list, removing it. */
        D2D::Figure * pop2DFigure();

        /* Retrives the figure from the beginning of the list, removing it. */
        D2D::Figure * shift2DFigure();

        /* Retrives the figure from the front of the list. Does not remove it. */
        D2D::Figure * first2DFigure();

        /* Retrieves the last figure from the list. It does not remove it. */
        D2D::Figure * last2DFigure();
};

#endif
