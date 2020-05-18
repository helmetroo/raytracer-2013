/**
 * Samuel Sweet
 * CS 5600-1 Assignment 5
 *
 * Represents a sphere.
 */

#include "sphere.h"

// Returns intersection T.
float raySphereTest(Ray* ray, Sphere* sphere)
{
    // Calculate pieces of the intersection formula.
   Vector3D eyeCenterDirec = subtractVec3D(ray->pointOnRay, sphere->centerPoint);
   float eyeCenterDirecDot = dotProd(eyeCenterDirec, eyeCenterDirec);
   float radiusSquared = sphere->radius * sphere->radius;
   float directionDotDirection = dotProd(ray->direction, ray->direction);
   float minusDirectionEyeCenter = dotProd(scalarMult(-1.0f, ray->direction), eyeCenterDirec);
   float directionEyeCenter = dotProd(ray->direction, eyeCenterDirec);
   
   // calculate discrim before calculating other terms.
   float discriminant = (directionEyeCenter*directionEyeCenter) -
                             (directionDotDirection*(eyeCenterDirecDot - radiusSquared));

   if(discriminant < 0.0f)
      return INF;
   
   float sqrtDiscriminant = sqrt(discriminant);
   
   // Two solutions, most important being the closest to the camera, so we only calculate it.
   float minusIntersectT = (minusDirectionEyeCenter-sqrtDiscriminant) / directionDotDirection;
   
   return minusIntersectT;
}
