/**
 * This file contains the description for the camera element, which will made all the
 * modelview transformations.
 * The content will be the abstract camera and the static camera.
 *
 * @author  Roberto Fernandez Cueto
 * @date    26.11.2015
 *
 * $Id$
 */

#ifndef _CAMERA_H
#define _CAMERA_H
#include "geometry.h"

/* If the gengine.h is not the included one, add the inclusion to the namespace. */
#ifndef _GENGINE_H
namespace GEngine {
    class Camera;
    class StaticCamera;
};
#endif

/**
 * Define the generic camera class.
 */
class GEngine::Camera {
    friend class StaticCamera;
    protected:
        Geometry::Point       position;   /* The position of the camera in the space. */
        double      yaw, pitch, roll;   /* The inclination of the camera in the three axis. */
    public:
        /* Constructor, initial to the origin. */
        Camera(Geometry::Point initial = Geometry::Point(), double yaw = 0.0, double pitch = 0.0, double roll = 0.0);

        /* Move the camera to this point. */
        void move(Geometry::Point point);

        /* Set the inclination of the camera. */
        void rotate(double yaw, double pitch, double roll);

        /* Controls the camera. Needs to be overriden. */
        virtual int cameraCtrl(void * data) = 0;

        /* Active the camera. */
        void activate();

        /* Deactivate the camera. */
        void deactivate();

        /* Get the distance of the point to the camera. */
        double getDistance(Geometry::Point p);

        /* Get direction. */
        Vector getDirection();
};

/**
 * Define a static camera.
 */
class GEngine::StaticCamera : public Camera {
    public:
        /* Constructor, initial to the origin. */
        StaticCamera(Geometry::Point initial = Geometry::Point(), 
                double yaw = 0.0, double pitch = 0.0, double roll = 0.0);
        StaticCamera(const Camera& cam);

        int cameraCtrl(void *data);
};

#endif
