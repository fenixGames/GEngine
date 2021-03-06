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


Material Scene::black = Material();
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
    camera = NULL;
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
    camera = NULL;
}

/**
 * Adds a figure to the list of all the dynamic figures on the scene.
 * @param   Figure  * fig   The figure to attach to the list.
 */
void
Scene::addDynFigure(Figure * fig)
{
    DynFigures.push_back(fig);
}

/**
 * Adds a figure to the list of all the static figures.
 * @param   StaticFigure    *fig    The static figure that must be added.
 */
void
Scene::addStaFigure(StaticFigure * fig)
{
    StaFigures.push_back(fig);
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
 * Prints the dynamic figures on the screen.
 */
void
Scene::printDynamic()
{
    FaceList                * faces;
    FaceList::iterator      faceIt;
    PointList::iterator     pointIter;
    FigureList::iterator    iter;

    /* Going through the list of DynFigures. */
    for (iter = DynFigures.begin(); iter != DynFigures.end(); iter++) {
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

/**
 * Prints the static figures on the screen.
 */
void
Scene::printStatic()
{
    FaceList                * faces;
    FaceList::iterator      faceIt;
    PointList::iterator     pointIter;
    StaticFigureList::iterator    iter;

   /* Going through the list of static figures. */
    for (iter = StaFigures.begin(); iter != StaFigures.end(); iter++) {
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

/**
 * Prints the whole scene on the screen.
 */
void
Scene::print()
{
    LightList::iterator     liter;

    /* Print the horizon (it is a skybox). */
    /** A skybox is a texture loaded from 6 files, each one for the 
     * GL_TEXTURE_CUBE_MAP_<POSITIVE|NEGATIVE>_<X|Y|Z> componentes of
     * the GL_TEXTURE_CUBE_MAP texture.
     * go to https://ogldev.atspace.co.uk/www/tutorial25/tutorial25.html
     * for more information.
     */

    /* If there is no camera, nothing will be recorded. */
    if (camera == NULL)
        return;
    else
        camera->setFOV(0.1, 90, limits.zmax - limits.zmin);

    camera->activate();
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
 
    printDynamic();
    printStatic();

    /* Activating the lights of the scene. */
    for (liter = lights.begin(); liter != lights.end(); liter++)
        (*liter)->activate();

    camera->deactivate();
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

    if ((int)lights.size() < maxLights)
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

/**
 * Process the scene in the idle state (not printing).
 * @param   double  time    The time since the program was launched.
 */
void
Scene::idle(const double time)
{
    FigureList::iterator fig;

    for (fig = DynFigures.begin(); fig != DynFigures.end(); fig++)
        (*fig)->motion(time);
    if (camera != NULL)
        camera->cameraCtrl(time, NULL);
}

/**
 * Sets the camera for the scene.
 * @param   Camera  cam     The camera to be set into the screen.
 */
void
Scene::addCamera(Camera * cam)
{
    camera = cam;
}

