#include "display.h"
#include <string.h>
//#include "probes.h"

Display *Display::theDisplay = NULL;

/**
 * Inititializes the display class and the main window's size and position.
 *
 * @param int       *argc   The pointer to the original number of arguments of the main function.
 * @param char      **argv  The array of arguments passed to the program.
 * @param GLuint    width   The width of the main window for the application.
 * @param GLuint    height  The height of the main window for the application.
 * @param GLuint    x       The horizontal position of the window on the screen.
 * @param GLuint    y       The vertical position of the window on the screen.
 */
//Display::Display(int * argc, char ** argv, GLuint width, GLuint height, GLuint x, GLuint y)
//{
//    /* Setting the components of this class. */
//    this->screen[0] = width;
//    this->screen[1] = height;
//
//    this->position[0] = x;
//    this->position[1] = y;
//
//    this->bgcolor[0] = 1.0f;
//    this->bgcolor[1] = 1.0f;
//    this->bgcolor[2] = 1.0f;
//
//    this->fgcolor[0] = 0.0f;
//    this->fgcolor[1] = 0.0f;
//    this->fgcolor[2] = 0.0f;
//
//    /* Initialazing the title of the window to NULL. */
//    this->title = NULL;
//
//    if (this->theDisplay == NULL) {
//        /* GLUT initialization. */
//        glutInit(argc, argv);
//        
//        this->theDisplay = this;
//    }
//}

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

    /* Initialazing the title of the window to NULL. */
    this->title = NULL;

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
    FigureList::iterator iter;  /* The iterator of the figure list to print them. */
    FigureList *list;            /* Shortcut for Display::theDisplay->figures */

    /* Getting the figure list. */
    if (Display::theDisplay != NULL) {
        list = &Display::theDisplay->figures;
        iter = list->begin();
    } else
        list = NULL;

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

    /* Prints the figures of the list. */
    for (; list != NULL && iter != list->end(); iter++) {
        (* iter)->print();
    }

    glPointSize(4.0f);


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

/**
 * Adds a figure object to the list of objects to be printed.
 * @param Figure    *figure     The figure to be added to the list.
 */
void
Display::addFigure(Figure * figure)
{
    this->figures.push_back(figure);
}

/**
 * Removes the figure from the list of figures to print.
 * @param Figure    *figure     The figure to be removed.
 */
void
Display::removeFigure(Figure *figure)
{
    this->figures.remove(figure);
}

/**
 * Retrives the last item of the figure list and removes it.
 * @return The requested item.
 */
Figure *
Display::popFigure()
{
    Figure * item = this->figures.back();
    this->figures.pop_back();

    return item;
}

/**
 * Retrieves the first element of the list and removes it.
 * @return The requested element.
 */
Figure *
Display::shiftFigure()
{
    Figure * item = this->figures.front();
    this->figures.pop_front();

    return item;
}

/**
 * Retrieves the first item of the list and returns it.
 * @return The item requested.
 */
Figure *
Display::firstFigure()
{
    return this->figures.front();
}

/**
 * Retrieves the last item of the list and returns it.
 * @return The item requested.
 */
Figure *
Display::lastFigure()
{
    return this->figures.back();
}

