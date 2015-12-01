/**
 * Definition of the light class and its attributes.
 *
 * @author  Roberto Fernandez Cueto
 * @date    01.12.2015
 */

#ifndef _LIGHT_H
#define _LIGHT_H

#ifndef _GENGINE_H
namespace GEngine {
    class Light;
};
#endif

#include "geometry.h"

/**
 * Defines any kind of source of light, position, 
 * directional or spotlight (flashlight and lamps).
 */
class GEngine::Light {
    private:
        static unsigned nlights; /* Control the number of lights. */
    protected:
        static unsigned char ambient[4]; /* Ambient light for the scene. */
        unsigned char intA[4], 
                      intD[4],
                      intSP[4];
        float position[4];  /* The position of the source of light (even directional). */
        float spDir[3]; /* Spot light direction. */
        float spExp; /* Intensity distribution for the spot light. */
        float spAng; /* Opening angle for the spot light. */
        float atten[3]; /* Attenuation coefficients. */
    public:
};

#endif
