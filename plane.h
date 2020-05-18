/**
 * Samuel Sweet
 * CS 5600-1 Assignment 5
 *
 * Represents a non-infinite plane (quad).
 */

#ifndef PLANE_H
#define	PLANE_H

#include <math.h>

#include "ray.h"
#include "vector.h"
#include "material.h"
#include "const.h"

typedef struct _Plane {
    Vector3D swPoint;
    Vector3D nwPoint;
    Vector3D nePoint;
    Vector3D sePoint;
    
    Material material;
} Plane;

// Intersection methods
float rayPlaneTest(Ray* ray, Plane* plane);
static int pointTriangleTest(Vector3D* a, Vector3D* b, Vector3D* c, 
                                        Vector3D* point, int xIndex, int yIndex);

// Creates a plane. Point order is important, as the normals
// are calculated based off this order.
inline void initPlanefv(Plane* plane, float sw_x, float sw_y, float sw_z,
                                  float nw_x, float nw_y, float nw_z,
                                  float ne_x, float ne_y, float ne_z,
                                  float se_x, float se_y, float se_z, Material* material)
{
    initVector3D(&plane->swPoint, sw_x, sw_y, sw_z);
    initVector3D(&plane->nwPoint, nw_x, nw_y, nw_z);
    initVector3D(&plane->nePoint, ne_x, ne_y, ne_z);
    initVector3D(&plane->sePoint, se_x, se_y, se_z);
    
    copyMaterial(material, &plane->material);
}

// Calculates the normal for a plane.
inline Vector3D planeSurfaceNormal(Plane* plane)
{
    Vector3D firstVectorInPlane = subtractVec3D(plane->nwPoint, plane->swPoint);
    Vector3D secondVectorInPlane = subtractVec3D(plane->nePoint, plane->nwPoint);
    Vector3D crossProduct = crossProd(firstVectorInPlane, secondVectorInPlane);
    return norm(&crossProduct);
}

#endif	/* PLANE_H */

