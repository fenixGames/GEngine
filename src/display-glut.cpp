#include "display.h"
#include <GL/glut.h>

using namespace GEngine;

Display *Display::theDisplay = NULL;

/**
 * Wrapper for the GLUT dependent initialization of the screen.
 */
void
Display::displayInit()
{
    char *argv[1];
    char dummyString[8];
    int argc = 1;

    argv[0] = dummyString;
    glutInit( &argc, argv );
}

/**
 * Wrapper for the GLUT dependent function for swaping the buffers.
 */
void
Display::SwapBuffers()
{
    glutSwapBuffers();
}
/**
 * Prints everything on the screen.
 *
 * @return int  Returns 0 on success.
 */
int
Display::print()
{
	/* Setting the position and the size of the window. */
    glutInitWindowPosition(position[0], position[1]);
    glutInitWindowSize(screen[0], screen[1]);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
 
    /* Setting the title of the window. */
    if (title == NULL)   
        mainWin = glutCreateWindow("");
    else
        mainWin = glutCreateWindow(title);

    /* Setting the function to redraw everything. */
    glutDisplayFunc(&Display::displayFunc);

	/* Main loop should be on the GEngine class when finished. */
	glutMainLoop();
    return 0;
}

