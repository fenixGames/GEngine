/**
 * Definition of the scenes, the regions, maps, worlds and universe.
 *
 * @author  Roberto Fernandez Cueto
 * @date    03.12.2015
 */

#include <GL/gl.h>
#include "world.h"
#include <math.h>
#ifdef DEBUG
#include <stdio.h>
#endif

using namespace GEngine;
using namespace GEngine::Geometry;

const int _black = 0;

Material Scene::black = Material(Material::GL_SOLID_COLOR, (void *) &_black);
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

/**
 * Prints the scene on the screen.
 */
void
Scene::print()
{
    FaceList                * faces;
    FaceList::iterator      faceIt;
    PointList::iterator     pointIter;
    FigureList::iterator    iter;
    LightList::iterator     liter;

    /* Print the horizon (it is a skybox). */
    /** A skybox is a texture loaded from 6 files, each one for the 
     * GL_TEXTURE_CUBE_MAP_<POSITIVE|NEGATIVE>_<X|Y|Z> componentes of
     * the GL_TEXTURE_CUBE_MAP texture.
     * go to https://ogldev.atspace.co.uk/www/tutorial25/tutorial25.html
     * for more information.
     */

#ifdef DEBUG
    long long xstep, zstep;
    /* Print the XZ plane as lines for debug. */
    glBegin(GL_LINES);
    for (xstep = limits.xmin; xstep < limits.xmax; xstep += 10) {
        glVertex3d(xstep, limits.ymin, limits.zmin);
        glVertex3d(xstep, limits.ymin, limits.zmax);
    }

    for (zstep = limits.zmin; zstep < limits.zmax; zstep += 10) {
        glVertex3d(limits.xmin, limits.ymin, zstep);
        glVertex3d(limits.xmax, limits.ymin, zstep);
    }
    glEnd();
#endif

    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambient);
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

    /* Activating the lights of the scene. */
    for (liter = lights.begin(); liter != lights.end(); liter++)
        (*liter)->activate();
}

/**
 * Adds a light source to the scene if the maximum number of lights have not been reached.
 * @param   Light   light       The light to be added to the scene.
 */
void
Scene::addLight(Light light)
{
    int maxLights;
    Light * lamp = new Light(light);

    lamp->idx = lights.size();
    glGetIntegerv(GL_MAX_LIGHTS, &maxLights);

    if (lights.size() < maxLights)
        lights.push_back(lamp);
    else
        delete lamp;
}

/**
 * Sets the ambient light for the scene.
 * @param   double  red     The red component of the scene.
 * @param   double  green   The green component of the scene.
 * @param   double  blue    The blue component of the scene.
 */
void
Scene::setAmbient(float red, float green, float blue)
{
    ambient[0] = red;
    ambient[1] = green;
    ambient[2] = blue;
    ambient[3] = 1.0;
}
