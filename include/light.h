/**
 * Definition of the light class and its attributes.
 *
 * @author  Roberto Fernandez Cueto
 * @date    01.12.2015
 */

#ifndef _LIGHT_H_
#define _LIGHT_H_

namespace GEngine {
    class Light;
};

#include "geometry.h"

/**
 * Defines any kind of source of light, position, 
 * directional or spotlight (flashlight and lamps).
 */
class GEngine::Light {
    friend class Scene;
    private:
        void setDefaults();
    protected:
        float   intA[4], 
                intD[4],
                intSP[4];
        float position[4];  /* The position of the source of light (even directional). */
        float spDir[3]; /* Spot light direction. */
        float spExp; /* Intensity distribution for the spot light. */
        float spAng; /* Opening angle for the spot light. */
        float atten[3]; /* Attenuation coefficients. */

        unsigned int idx; /* Should be set by the scene. */
    public:
        /* Set a positional light. */
        Light(Geometry::Point pos = Geometry::Point());

        /* Set a directional light. */
        Light(Vector vect);

        /* Sets the intensity. */
        void setIntensity(float amb_red = 0.5, float amb_green = 0.5, float amb_blue = 0.5,
                float diff_red = 0.5, float diff_green = 0.5, float diff_blue = 0.5,
                float spec_red = 0.5, float spec_green = 0.5, float spec_blue = 0.5);

        /* Sets the intensity for the ambient light. */
        static void setAmbient(unsigned char red, unsigned char green, unsigned char blue);

        /* Sets the spot light properties. */
        void setSpotLight(Vector dir, float dist, float ang);

        /* Sets the attenuation coefficients. */
        void setAttenuation(float constant, float linear, float cuadratic);

        /* Activates/Deactivates the light. */
        void activate();
        void deactivate();
};

#endif
