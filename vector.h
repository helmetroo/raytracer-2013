/**
 * Samuel Sweet
 * CS 5600-1 Assignment 5
 *
 * Represents a three-dimensional vector.
 */

#ifndef VECTOR_H
#define VECTOR_H

#include <math.h>

typedef struct _Vector3D {
   float x;
   float y;
   float z;
} Vector3D;

// Every basic operation is inlined to avoid overhead from creating a stack frame.
inline void initVector3D(Vector3D* toInit, float x, float y, float z);
inline void copyVector3D(Vector3D* src, Vector3D* dest);
inline void normInPlace(Vector3D* toNormalize);
inline Vector3D addVec3D(Vector3D first, Vector3D second);
inline Vector3D subtractVec3D(Vector3D first, Vector3D second);
inline Vector3D scalarMult(float scalar, Vector3D vector);
inline Vector3D reflect(Vector3D v, Vector3D n);
inline float dotProd(Vector3D first, Vector3D second);
inline Vector3D crossProd(Vector3D first, Vector3D second);
inline float vecAtIndice(Vector3D* vec, int index) {  return *(((float*)vec) + index);   }

// Defines a vector.
inline void initVector3D(Vector3D* toInit, float x, float y, float z)
{
   toInit->x = x;
   toInit->y = y;
   toInit->z = z;
}

// Copies a vector to another.
inline void copyVector3D(Vector3D* src, Vector3D* dest)
{
   dest->x = src->x;
   dest->y = src->y;
   dest->z = src->z;
}

// Directly normalizes and modifies the vector.
inline void normInPlace(Vector3D* toNormalize)
{
   float overLength = 1.0f / sqrt( toNormalize->x*toNormalize->x +
				   toNormalize->y*toNormalize->y +
				   toNormalize->z*toNormalize->z );

   toNormalize->x *= overLength;
   toNormalize->y *= overLength;
   toNormalize->z *= overLength;
}

// Normalizes a vector.
inline Vector3D norm(Vector3D* toNormalize)
{
    Vector3D norm;
    
    float overLength = 1.0f / sqrt( toNormalize->x*toNormalize->x +
				    toNormalize->y*toNormalize->y +
				    toNormalize->z*toNormalize->z );

    initVector3D(&norm, toNormalize->x * overLength, 
                              toNormalize->y * overLength,
                                toNormalize->z * overLength);
   
    return norm;
}

// Adds a vector to another.
inline Vector3D addVec3D(Vector3D first, Vector3D second)
{
   Vector3D result;

   initVector3D(&result, 
		first.x + second.x, 
		first.y + second.y, 
		first.z + second.z);

   return result;
}
// Subtracts a vector from another.
inline Vector3D subtractVec3D(Vector3D first, Vector3D second)
{
   Vector3D result;

   initVector3D(&result, 
		first.x - second.x, 
		first.y - second.y, 
		first.z - second.z);

   return result;
}

// Multiplies a vector by a scalar.
inline Vector3D scalarMult(float scalar, Vector3D vector)
{
   Vector3D result;

   initVector3D(&result,
		scalar*vector.x,
		scalar*vector.y,
		scalar*vector.z);

   return result;
}

// Reflects a vector.
inline Vector3D reflect(Vector3D normal, Vector3D direction)
{
   return subtractVec3D( direction, scalarMult(2.0f, scalarMult(dotProd(direction, normal), normal)) );
}

// Dot product
inline float dotProd(Vector3D first, Vector3D second)
{
   return first.x*second.x + first.y*second.y + first.z*second.z;
}

// Cross product
inline Vector3D crossProd(Vector3D first, Vector3D second)
{
    Vector3D newVec;
    initVector3D(&newVec, first.y*second.z - first.z*second.y, 
                           first.z*second.x - first.x*second.z, 
                             first.x*second.y - first.y*second.x);
    
    return newVec;
}

#endif
