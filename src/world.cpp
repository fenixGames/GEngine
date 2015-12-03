/**
 * Definition of the scenes, the regions, maps, worlds and universe.
 *
 * @author  Roberto Fernandez Cueto
 * @date    03.12.2015
 */

#include <GL/gl.h>
#include "world.h"
#include <math.h>

using namespace GEngine;
using namespace GEngine::Geometry;

static int black = 0;

Material Scene::black = Material(Material::GL_SOLID_COLOR, (void *) &black);
/**
 * Constructors of the scene.
 */
Scene::Scene(long long xmin, long long xmax, long long ymin, long long ymax, 
        long long zmin, long long zmax)
{
    limits.xmin = xmin;
    limits.xmax = xmax;
    limits.ymin = ymin;
    limits.ymax = ymax;
    limits.zmin = zmin;
    limits.zmax = zmax;

    horizon = &black;
}

Scene::Scene(long long lim[6])
{
    limits.xmin = lim[0];
    limits.xmax = lim[0];
    limits.ymin = lim[0];
    limits.ymax = lim[0];
    limits.zmin = lim[0];
    limits.zmax = lim[0];

    horizon = &black;
}

/**
 * Adds a figure to the list of all the figures on the scene.
 * @param   Figure  * fig   The figure to attach to the list.
 */
void
Scene::addFigure(Figure * fig)
{
    figures.push_back(fig);
}

/**
 * Sets the horizon's material instead of the black one.
 * @param   Material    *hor    The material for the horizon.
 */
void
Scene::setHorizon(Material * hor)
{
    horizon = hor;
}


void
Scene::print()
{
    FaceList                * faces;
    FaceList::iterator      faceIt;
    PointList::iterator     pointIter;
    FigureList::iterator    iter;
    float   alpha, beta;
    double dist;
    bool upPoint;

#ifdef DEBUG
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    axiscam.move(Point());

    axiscam.activate();
	glBegin(GL_LINES);
    glColor3d( 1.0, 0.0, 0.0);
    glVertex3d(-1.0, 0.0, 0.0);
    glVertex3d( 1.0, 0.0, 0.0);
    
    glColor3d(0.0, 1.0, 0.0);
    glVertex3d(0.0, -1.0, 0.0);
    glVertex3d(0.0,  1.0, 0.0);
    
    glColor3d(0.0, 0.0, 1.0);
    glVertex3d(0.0, 0.0, -1.0);
    glVertex3d(0.0, 0.0, 1.0);
    glEnd();

    axiscam.deactivate();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
#endif

    dist = (limits.xmax - limits.xmin) * (limits.xmax - limits.xmin);
    dist += (limits.ymax - limits.ymin) * (limits.ymax - limits.ymin);
    dist += (limits.zmax - limits.zmin) * (limits.zmax - limits.zmin);

    dist = sqrt(dist);
    /* Print the horizon (it is a sphere with the horizon's material. */
    horizon->activate();
    glBegin(GL_TRIANGLE_STRIP);
    for (alpha = 0; alpha < 2.0 * M_PI; alpha += M_PI / 90.0) {
        upPoint = false;
        for (beta = 0; beta < M_PI; beta += M_PI / 90.0) {
            glTexCoord2d(alpha / (2.0 * M_PI), beta / M_PI);
            glVertex3d(dist * cos(alpha) * cos(beta), dist * sin(alpha) * cos(beta), dist * sin(beta));
        }
    }

    horizon->deactivate();

    /* Going through the list of figures. */
    for (iter = figures.begin(); iter != figures.end(); iter++) {
        faces = (*iter)->print();
        
        (*iter)->activeMaterial();

        /* Going through each of the faces. */
        for (faceIt = faces->begin(); faceIt != faces->end(); faceIt++) {
            glBegin((*iter)->getMode());

            /* Printing the points. */
            for (pointIter = (*faceIt)->vertex->begin(); pointIter != (*faceIt)->vertex->end(); pointIter++){
    			/* Printing the points. */
                glTexCoord2d((*pointIter)->s, (*pointIter)->t);
                glVertex3d((*pointIter)->x, (*pointIter)->y, - (*pointIter)->z);
            }

            glEnd();
        }
        delete faces;
        (*iter)->deactivateMaterial();
    }

}

