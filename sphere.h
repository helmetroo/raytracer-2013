/**
 * Samuel Sweet
 * CS 5600-1 Assignment 5
 *
 * Represents a sphere.
 */

#ifndef SPHERE_H
#define SPHERE_H

#include <math.h>

#include "vector.h"
#include "ray.h"
#include "material.h"
#include "const.h"

// Comprised of center point, radius, and material
typedef struct _Sphere {
   Vector3D centerPoint;
   float radius;

   Material material;
} Sphere;

// Tests an intersection between a ray and sphere.
float raySphereTest(Ray* ray, Sphere* sphere);

// Creates a sphere
inline void initSphere(Sphere* sphere, float cX, float cY, float cZ, float r, Material* material)
{
   sphere->centerPoint.x = cX;
   sphere->centerPoint.y = cY;
   sphere->centerPoint.z = cZ;
   
   sphere->radius = r;
   
   copyMaterial(material, &sphere->material);
}

// Returns a normal vector at a point on the sphere.
inline Vector3D sphereSurfaceNormal(Sphere* sphere, Vector3D* intersectPoint)
{
   Vector3D normalVec = subtractVec3D(*intersectPoint, sphere->centerPoint);
   return norm(&normalVec);
}

#endif
