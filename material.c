/**
 * Samuel Sweet
 * CS 5600-1 Assignment 5
 *
 * A material (ambient, diffuse, specular color).
 */

#include "material.h"

Color checkerboardMap(float x, float z)
{
    Color checkerboardDiffuse;
    
    // Scaling in one direction affects size of checkerboard cells.
    int xzPosition = (int)floor(x) + (int)floor(2.0f*z);
    
    if(xzPosition % 2)
        initColor(&checkerboardDiffuse, 0.0f, 0.0f, 0.0f, 1.0f);
    else
        initColor(&checkerboardDiffuse, 0.25f, 0.75f, 1.0f, 1.0f);
    
    return checkerboardDiffuse;
}
