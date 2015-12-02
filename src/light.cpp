/**
 * Definition of the light class and its attributes.
 *
 * @author  Roberto Fernandez Cueto
 * @date    01.12.2015
 */

#include <GL/gl.h>
#include "light.h"
//#include <string.h>

using namespace GEngine;
using namespace GEngine::Geometry;

unsigned int Light::nlights = 0;

/**
 * Constructors of the Light class.
 */
Light::Light(Point pos)
{
    position[0] = pos.x;
    position[1] = pos.y;
    position[2] = pos.z;
    position[3] = 1.0;

    setDefaults();
}

Light::Light(Vector vect)
{
    position[0] = vect[0];
    position[1] = vect[1];
    position[2] = vect[2];
    position[3] = 0.0;

    setDefaults();
}

/**
 * Sets the defaults values for the parameters.
 */
void
Light::setDefaults()
{
    int maxLights;

    /* Get the maximum number of light objects. */
    glGetIntegerv(GL_MAX_LIGHTS, &maxLights);

    /* Set the identifier of the light. */
    idx = GL_LIGHT0 + nlights;
    nlights = (nlights + 1) % maxLights;

    intA[0] = intA[1] = intA[2] = 0.0;
    intD[0] = intD[1] = intD[2] = 0.0;
    intSP[0] = intSP[1] = intSP[2] = 0.0;

    intA[3] = intD[3] = intSP[3] = 1.0;

    spDir[0] = spDir[1] = 0.0;
    spDir[2] = -1.0;

    spExp = 0;
    spAng = 180;

    atten[0] = 1.0;
    atten[1] = atten[2] = 0.0;
}

/**
 * Sets the intensity variables for the light.
 * @param   float   ar  The red component of the ambient light.
 * @param   float   ag  The green component of the ambient light.
 * @param   float   ab  The blue component of the ambient light.
 * @param   float   dr  The red component of the diffuse light.
 * @param   float   dg  The green component of the diffuse light.
 * @param   float   db  The blue component of the diffuse light.
 * @param   float   sr  The red component of the specular light.
 * @param   float   sg  The green component of the specular light.
 * @param   float   sb  The blue component of the specular light.
 */
void
Light::setIntensity(float ar, float ag, float ab, float dr, float dg,
        float db, float sr, float sg, float sb)
{
    intA[0] = ar; intA[1] = ag; intA[2] = ab;
    intD[0] = dr; intD[1] = dg; intD[2] = db;
    intSP[0] = sr; intSP[1] = sg; intSP[2] = sb;
}

/**
 * Sets the properties of a spot light.
 * @param   Vector  dir     The direction of the spot light.
 * @param   float   dist    The distribution coefficient.
 * @param   float   ang     The opening angle of the spot light.
 */
void
Light::setSpotLight(Vector dir, float dist, float ang)
{
    spDir[0] = dir[0]; spDir[1] = dir[1]; spDir[2] = dir[2];

    spExp = dist;
    spAng = ang;
}

/**
 * Sets the attenuation coefficients.
 * @param   float   constant    The constant component.
 * @param   float   linear      The linear component.
 * @param   float   cuadratic   The cuadratic component.
 */
void
Light::setAttenuation(float constant, float linear, float cuadratic)
{
    atten[0] = constant;
    atten[1] = linear;
    atten[2] = cuadratic;
}

