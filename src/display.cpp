#include "display.h"
//#include "probes.h"

Display *Display::theDisplay = NULL;

/**
 * Inititializes the display class and the main window's size and position.
 *
 * @param GLuint    width   The width of the main window for the application.
 * @param GLuint    height  The height of the main window for the application.
 * @param GLuint    x       The horizontal position of the window on the screen.
 * @param GLuint    y       The vertical position of the window on the screen.
 */
Display::Display(int * argc, char ** argv, GLuint width, GLuint height, GLuint x, GLuint y)
{
    /* Setting the components of this class. */
    this->screen[0] = width;
    this->screen[1] = height;

    this->position[0] = x;
    this->position[1] = y;

    this->color[0] = 1.0f;
    this->color[1] = 1.0f;
    this->color[2] = 1.0f;

    /* Tracing and setting the default position and size of the display. */
    glutInitWindowSize(width, height);
    glutInitWindowPosition(x, y);
    glutInit(argc, argv);

    if (this->theDisplay == NULL)
        this->theDisplay = this;
}

/**
 * Draws everything on the screen.
 */
void
Display::displayFunc()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glutSwapBuffers(); 
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
    this->color[0] = (GLfloat)(r / 255);
    this->color[1] = (GLfloat)(g / 255);
    this->color[2] = (GLfloat)(b / 255);
}

/**
 * Prints everything on the screen.
 *
 * @return int  Returns 0 on success.
 */
int
Display::print()
{
    this->mainWin = glutCreateWindow("");

    /* Setting the background color. */
    glClearColor(this->color[0], this->color[1], this->color[2], 0.0f);

    /* Setting the function to redraw everything. */
    glutDisplayFunc(&Display::displayFunc);

    glutMainLoop();
    return 0;
}

