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

#ifndef _CAMERA_H_
#define _CAMERA_H_
#include "geometry.h"

/* If the gengine.h is not the included one, add the inclusion to the namespace. */
namespace GEngine {
    class Camera;
    class StaticCamera;
};

/**
 * Define the generic camera class.
 */
class GEngine::Camera {
    friend class StaticCamera;
    protected:
        Geometry::Point       position;   /* The position of the camera in the space. */
        double      yaw, pitch, roll;   /* The inclination of the camera in the three axis. */
        double      projection[6];      /* The values for the projection matrix. */
    public:
        /* Constructor, initial to the origin. */
        Camera(Geometry::Point initial = Geometry::Point(), double yaw = 0.0, double pitch = 0.0, double roll = 0.0);

        /* Move the camera to this point. */
        void move(Geometry::Point point);

        /* Set the inclination of the camera. */
        void rotate(double yaw, double pitch, double roll);

        /* Controls the camera. Needs to be overriden. */
        virtual int cameraCtrl(double time, void * data = NULL) = 0;

        /* Active the camera. */
        void activate();

        /* Deactivate the camera. */
        void deactivate();

        /* Sets the field of view for the camera. */
        void setFOV(double distance, double overture, double depth);
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

        int cameraCtrl(double time, void *data = NULL);
};

#endif
