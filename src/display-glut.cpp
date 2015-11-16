#include "display.h"
#include <string.h>
#include <math.h>

using namespace GEngine;

Display *Display::theDisplay = NULL;

/* Declaring the external static funstions to draw the 2D and 3D figures. */
extern int print2D(Figure2DList * list, int winId, Matrix * trans);

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
	double ax, ay;

    /* Setting the components of this class. */
    screen[0] = width;
    screen[1] = height;

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

    /* Initializing the lists of figures to NULL. */
    figures2D = NULL;
    //figures3D = NULL; TODO

	/* Settign the transformation matrix to the default values. */
	ax = 2.0 / (screen[0] - 1.0); /* Setting the X modifier. */
	ay = 2.0 / (screen[1] - 1.0); /* Setting the Y modifier. */
	trans = new Matrix(3, 3, ax, 0.0, -1.0, 0.0, ay, -1.0, 0.0, 0.0, 0.0);

	if (theDisplay == NULL) {
		/* GLUT initialization. */
		argv[0] = dummyString;
		glutInit(&argc, argv);

		theDisplay = this;
	}
}

Display::~Display()
{
	if (title != NULL)
		free(title);
	if (figures2D != NULL)
		delete figures2D;
}

/**
 * Draws everything on the screen.
 */
void
Display::displayFunc()
{
    Figure2DList *list2D = NULL;    /* Shortcut for Display::theDisplay->figures2D */
    /* TODO
    Figure3DList *list3D = NULL;    // Shortcut for Display::theDisplay->figures3D
    */

    /* Getting the figure list. */
    if (Display::theDisplay != NULL && Display::theDisplay->figures2D != NULL) {
        list2D = Display::theDisplay->figures2D;
    } 

    /* Cleans the screen. */
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glViewport(Display::theDisplay->position[0], Display::theDisplay->position[1],
            Display::theDisplay->screen[0], Display::theDisplay->screen[1]);
    glColor3f(Display::theDisplay->fgcolor[0],
            Display::theDisplay->fgcolor[1], Display::theDisplay->fgcolor[2]);

    /* Setting the Basic axis to debug the engine's printing. */
#ifdef DEBUG
	glPushAttrib(GL_ENABLE_BIT);
	glEnable(GL_LINE_STIPPLE);
	glLineStipple(1, 0xF0F0);
	glBegin(GL_LINES);
    glVertex2f(-1.0f, 0.0f);
    glVertex2f(1.0f, 0.0f);
    glVertex2f(0.0f, 1.0f);
    glVertex2f(0.0f, -1.0f);
    glEnd();

	glPopAttrib();
#endif

    glPointSize(4.0f);
    /* If the two list are set, print the 3D first, and then the 2D. */
    /* TODO 3D printing. */
    /* Prints the 2D figures of the list. */
    print2D(list2D, Display::theDisplay->mainWin, Display::theDisplay->trans);

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

    /* Setting the background color. */
    glClearColor(bgcolor[0], bgcolor[1], bgcolor[2], 0.0f);

    /* Setting the function to redraw everything. */
    glutDisplayFunc(&Display::displayFunc);

#ifdef DEBUG
    trans->print();
#endif

	/* Main loop should be on the GEngine class when finished. */
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
    title = (char *) malloc((++length) * sizeof(char));
    
    /* Copying the title. */
    if (title != NULL) {
        strncpy(title, _title, length);
    }

    return title != NULL;
}

/**
 * Rotate the whole display the angle indicated in the argument.
 *
 * @param	GLfloat	angle	The angle to rotate the display.
 */
void
Display::rotate(GLfloat angle) {
	/* Normalizing the angle (0 <= angle <= 360). */
	while (angle > 360.0f)
		angle -= 360.0f;

	while (angle < 0.0f)
		angle += 360.0f;

	angle *= M_PI / 180.0f;

	/* Declaring the rotational matrix. */
	Matrix rot(3, 3, cos(angle), -sin(angle), 0.0, sin(angle), cos(angle), 0.0, 0.0, 0.0, 1.0), * old;

	if (trans != NULL) {
		old = trans;
		/* Transforming the existent matrix. */
		(* trans) = (* old) * rot;
//		delete(old);
	}
}

