/**
 * Samuel Sweet
 * CS 5600-1 Assignment 5
 *
 * Represents a positional light source, defined by
 * an ambient color and position.
 */

#ifndef LIGHT_H
#define LIGHT_H

#include "color.h"
#include "vector.h"

typedef struct _Light {
   Color ambient;
   Vector3D pos;
} Light;

// Creates the light using known floating point values to represent color and position.
inline void initLightff(Light* light, float r, float g, float b, float a, 
	       float posX, float posY, float posZ)
{
   initColor(&light->ambient, r, g, b, a);
   initVector3D(&light->pos, posX, posY, posZ);
}

// Creates the light using known floating point values to represent color and vector to represent position.
inline void initLightfv(Light* light, float r, float g, float b, float a, Vector3D *pos)
{
   initColor(&light->ambient, r, g, b, a);
   initVector3D(&light->pos, pos->x, pos->y, pos->z);
}



#endif
