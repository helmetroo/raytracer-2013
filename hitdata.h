/**
 * Samuel Sweet
 * CS 5600-1 Assignment 5
 *
 * Represents useful information about
 * an object that a ray has hit.
 */

#ifndef HITDATA_H
#define HITDATA_H

#include "vector.h"
#include "material.h"

typedef struct _HitData {
   Vector3D surfaceNormal;
   Vector3D intersectPoint;
   Material material;
   float intersectT;
} HitData;

inline void initHitData(HitData* data, Vector3D surfaceNormal, Vector3D intersectPoint, Material material, float t)
{
   copyVector3D(&surfaceNormal, &data->surfaceNormal);
   copyVector3D(&intersectPoint, &data->intersectPoint);
   copyMaterial(&material, &data->material); 
   data->intersectT = t;
}

#endif
