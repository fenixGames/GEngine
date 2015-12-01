/**
 * This file contains the functions for the camera element, which will made all the
 * modelview transformations.
 * The content will be the abstract camera and the static camera.
 *
 * @author  Roberto Fernandez Cueto
 * @date    26.11.2015
 *
 * $Id$
 */

#include "camera.h"
#include <GL/gl.h>
#include "matrix.h"
#include <math.h>
#ifdef DEBUG
#include <stdio.h>
#endif

using namespace GEngine;
using namespace GEngine::Geometry;

/**
 * Constructor of the camera.
 */
Camera::Camera(Point pos, double y, double p, double r)
{
    position = pos;
    yaw = y;
    pitch = p;
    roll = r;
}

/**
 * Translates the camera to the new point.
 * @param   Point   point   The new point to be translated.
 */
void
Camera::move(Point point)
{
    /* Set the new point. */
    position = point;
}

/**
 * Rotates the camera using the yaw, pitch and roll angles.
 * @param   double  y   The yaw of the camera.
 * @param   double  p   The pitch of the camera.
 * @param   double  r   The roll of the camera.
 */
void
Camera::rotate(double y, double p, double r)
{
    yaw = y; pitch = y; roll = r;
}

/**
 * Makes the camera active.
 */
void
Camera::activate()
{
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();

    glLoadIdentity();
    glTranslated(position.x, position.y, position.z);
    glRotated(yaw, 1.0, 0.0, 0.0);
    glRotated(pitch, 0.0, 1.0, 0.0);
    glRotated(roll, 0.0, 0.0, 1.0);
}

/**
 * Makes the camera inactive.
 */
void
Camera::deactivate()
{
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
}

/**
 * Gets the distance from the point to the camera.
 */
double
Camera::getDistance(Point point)
{
    Vector cam(3, position.x, position.y, position.z),
           vect(3, point.x, point.y, point.z), out;

    out = cam - vect;

    return out.mod();
}

/**
 * Gets the direction in which the camera is recording.
 * @return  The vector indicating where the camera is pointing to.
 */
Vector
Camera::getDirection()
{
    Vector dir(3, 0.0, 0.0, -1.0);
    Matrix m_roll, m_yaw, m_pitch, rotation;

    m_roll = Matrix(3, 3,
            cos(roll * M_PI / 180.0), -sin(roll * M_PI / 180.0), 0.0,
            sin(roll * M_PI / 180.0), cos(roll * M_PI / 180.0), 0.0,
            0.0, 0.0, 1.0);
    m_pitch = Matrix(3, 3,
            cos(pitch * M_PI / 180.0), 0.0, sin(pitch * M_PI / 180.0),
            0.0, 1.0, 0.0,
            -sin(pitch * M_PI / 180.0), 0.0, cos(pitch * M_PI / 180.0));
    m_yaw = Matrix(3, 3,
            1.0, 0.0, 0.0,
            0.0, cos(yaw * M_PI / 180.0), -sin(yaw * M_PI / 180.0),
            0.0, sin(yaw * M_PI / 180.0), cos(yaw * M_PI / 180.0));

    rotation = m_roll * m_pitch * m_yaw;

    return rotation * dir;
}

/**
 * Constructor of the static camera.
 */
StaticCamera::StaticCamera(Point initial, double y, double p, double r) : Camera(initial, y, p, r)
{
}

StaticCamera::StaticCamera(const Camera& cam)
{
    position = cam.position;
    yaw = cam.yaw;
    pitch = cam.pitch;
    roll = cam.roll;
}

/**
 * Defines the camera control for the static camera -> do nothing.
 */
int 
StaticCamera::cameraCtrl(void *data)
{
    return 0;
}

