#include "display.h"
#include <string.h>

using namespace GEngine;

Display *Display::theDisplay = NULL;

/* Declaring the external static funstions to draw the 2D and 3D figures. */
extern int print2D(Figure2DList * list, int winId);

/**
 * Inititializes the display class and the main window's size and position.
 *
 * @param GLuint    width   The width of the main window for the application.
 * @param GLuint    height  The height of the main window for the application.
 * @param GLuint    x       The horizontal position of the window on the screen.
 * @param GLuint    y       The vertical position of the window on the screen.
 */
Display::Display(GLuint width, GLuint height, GLuint x, GLuint y)
{
	char *argv[1];
	char dummyString[8];
	int argc = 1;

    /* Setting the components of this class. */
    this->screen[0] = width;
    this->screen[1] = height;

    this->position[0] = x;
    this->position[1] = y;

    this->bgcolor[0] = 1.0f;
    this->bgcolor[1] = 1.0f;
    this->bgcolor[2] = 1.0f;

    this->fgcolor[0] = 0.0f;
    this->fgcolor[1] = 0.0f;
    this->fgcolor[2] = 0.0f;

    /* Initializing the title of the window to NULL. */
    this->title = NULL;

    /* Initializing the lists of figures to NULL. */
    this->figures2D = NULL;
    //this->figures3D = NULL; TODO

	if (this->theDisplay == NULL) {
		/* GLUT initialization. */
		argv[0] = dummyString;
		glutInit(&argc, argv);

		this->theDisplay = this;
	}
}

/**
 * Draws everything on the screen.
 */
void
Display::displayFunc()
{
    Figure2DList::iterator iter2D;  /* The iterator of the figure list to print them. */
    Figure2DList *list2D = NULL;    /* Shortcut for Display::theDisplay->figures2D */
    /* TODO
    Figure3DList::iterator iter3D;  // The iterator for the 3D figure list to print them.
    Figure3DList *list3D = NULL;    // Shortcut for Display::theDisplay->figures3D
    */

    /* Getting the figure list. */
    if (Display::theDisplay != NULL && Display::theDisplay->figures2D != NULL) {
        list2D = Display::theDisplay->figures2D;
        iter2D = list2D->begin();
    } 

    /* Cleans the screen. */
    glClear(GL_COLOR_BUFFER_BIT);
    glViewport(Display::theDisplay->position[0], Display::theDisplay->position[1],
            Display::theDisplay->screen[0], Display::theDisplay->screen[1]);
    glColor3f(Display::theDisplay->fgcolor[0],
            Display::theDisplay->fgcolor[1], Display::theDisplay->fgcolor[2]);

    /* Setting the Basic axis to debug the engine's printing. */
#ifdef DEBUG
    glBegin(GL_LINES);
    glVertex2f(-1.0f, 0.0f);
    glVertex2f(1.0f, 0.0f);
    glVertex2f(0.0f, 1.0f);
    glVertex2f(0.0f, -1.0f);
    glEnd();
#endif

    glPointSize(4.0f);
    /* If the two list are set, print the 3D first, and then the 2D. */
    /* TODO 3D printing. */
    /* Prints the 2D figures of the list. */
    print2D(list2D, Display::theDisplay->mainWin);

    /* Swaps the buffers so the printing will be visible. */
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
    this->bgcolor[0] = (GLfloat)(r / 255);
    this->bgcolor[1] = (GLfloat)(g / 255);
    this->bgcolor[2] = (GLfloat)(b / 255);
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
    this->fgcolor[0] = (GLfloat)(r / 255);
    this->fgcolor[1] = (GLfloat)(g / 255);
    this->fgcolor[2] = (GLfloat)(b / 255);
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
    glutInitWindowPosition(this->position[0], this->position[1]);
    glutInitWindowSize(this->screen[0], this->screen[1]);
 
    /* Setting the title of the window. */
    if (this->title == NULL)   
        this->mainWin = glutCreateWindow("");
    else
        this->mainWin = glutCreateWindow(this->title);

    /* Setting the background color. */
    glClearColor(this->bgcolor[0], this->bgcolor[1], this->bgcolor[2], 0.0f);

    /* Setting the function to redraw everything. */
    glutDisplayFunc(&Display::displayFunc);

    glutMainLoop();
    return 0;
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
    this->title = (char *) malloc((++length) * sizeof(char));
    
    /* Copying the title. */
    if (this->title != NULL) {
        strncpy(this->title, _title, length);
    }

    return this->title != NULL;
}


