/**
 * Samuel Sweet
 * CS 5600-1 Assignment 5
 *
 * Represents a ray (given by a point on it,
 * and the direction it goes in).
 */

#ifndef RAY_H
#define RAY_H

#include <math.h>

#include "vector.h"

typedef struct _Ray {
   Vector3D pointOnRay;
   Vector3D direction;
} Ray;

// Creates a ray without passing vectors.
inline void initRayf(Ray* ray, float pointX, float pointY, float pointZ,
		    float direcX, float direcY, float direcZ)
{
   ray->pointOnRay.x = pointX;
   ray->pointOnRay.y = pointY;
   ray->pointOnRay.z = pointZ;

   ray->direction.x = direcX;
   ray->direction.y = direcY;
   ray->direction.z = direcZ;
}

// Creates a ray using known vectors.
inline void initRayv(Ray* ray, Vector3D* point, Vector3D* direction)
{
    copyVector3D(point, &ray->pointOnRay);
    copyVector3D(direction, &ray->direction);
}

inline void copyRay(Ray* src, Ray* dest)
{
   dest->pointOnRay.x = src->pointOnRay.x;
   dest->pointOnRay.y = src->pointOnRay.y;
   dest->pointOnRay.z = src->pointOnRay.z;

   dest->direction.x = src->direction.x;
   dest->direction.y = src->direction.y;
   dest->direction.z = src->direction.z;
}

// Returns the point (or vector) at a given value along the ray.
inline Vector3D vectorAt(Ray ray, float t)
{
    return addVec3D(ray.pointOnRay, scalarMult(t, ray.direction));
}

#endif
