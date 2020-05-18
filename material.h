/**
 * Samuel Sweet
 * CS 5600-1 Assignment 5
 *
 * A material (ambient, diffuse, specular color).
 */

#ifndef MATERIAL_H
#define MATERIAL_H

#include <stdio.h>
#include <math.h>

#include "color.h"

// Function pointer allowing for a dynamic diffuse texture (based off position, say)
typedef Color (*TextureMap)(float, float);

typedef struct _Material {
  Color ambient;
  Color diffuse;
  Color specular;
  float specularCoeff;
  float reflection;
  TextureMap function;
} Material;

inline void copyMaterial(Material* src, Material* dest)
{
  copyColor(&src->ambient, &dest->ambient);
  copyColor(&src->diffuse, &dest->diffuse);
  copyColor(&src->specular, &dest->specular);
  
  dest->specularCoeff = src->specularCoeff;
  dest->reflection = src->reflection;
  
  dest->function = src->function;
}

inline void setNoFunction(Material* material)
{
    material->function = NULL;
}

inline void setFunction(Material* material, TextureMap function)
{
    material->function = function;
}

inline void setAmbient(Material* material, float r, float g, float b, float a)
{
  Color ambient;
  initColor(&ambient, r, g, b, a);
  material->ambient = ambient;
}

inline void setDiffuse(Material* material, float r, float g, float b, float a)
{
  Color diffuse;
  initColor(&diffuse, r, g, b, a);
  material->diffuse = diffuse;
  
  setNoFunction(material);
}

inline void setSpecular(Material* material, float r, float g, float b, float a)
{
  Color specular;
  initColor(&specular, r, g, b, a);
  material->specular = specular;
}

inline void setSpecularCoeff(Material* material, float shininess)
{
   material->specularCoeff = shininess;
}

inline void setReflection(Material* material, float reflection)
{
   material->reflection = reflection;
}

// Dynamic texture functions
Color checkerboardMap(float x, float z);


#endif
