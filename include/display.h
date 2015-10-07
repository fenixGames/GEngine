#ifndef _DISPLAY_H
#define _DISPLAY_H

#include <GL/gl.h>
#include <GL/glut.h>

class Display {
    protected:
        GLuint screen[2]; // Dimensions of the main window.
        GLuint position[2]; // Position of the main window.
        GLfloat color[3];  // The color of the background.

        static Display *theDisplay;
        int mainWin;

        /* The function to draw the screen. */
        static void displayFunc();
    public:
        Display(int *argc, char **argv, GLuint w, GLuint h, GLuint x = 0, GLuint y = 0);

        /* Prints everything on the screen. */
        int print();

        /* Sets the background color. */
        void bkgColor(unsigned char r, unsigned char g, unsigned char b);
};

#endif

