/**
 * Samuel Sweet
 * CS 5600-1 Assignment 5
 *
 * The raytracer itself, which can operate
 * on scene structs representing collections of lights and objects
 * that the raytracer uses to perform rendering.
 */

#ifndef RAYTRACER_H
#define RAYTRACER_H

#ifdef __APPLE__
#include "OpenGL/gl.h"
#include "OpenGL/glu.h"
#include "GLUT/glut.h"
#elif defined(_WIN32)
#include <windows.h>
#include "GL/glut.h"
#else
#include "GL/glut.h"
#endif

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "ray.h"
#include "plane.h"
#include "sphere.h"
#include "hitdata.h"
#include "light.h"
#include "color.h"
#include "vector.h"
#include "const.h"

// Constants
static const float EPSILON = 0.001f;

// A scene that the raytracer can render.
typedef struct _Scene {
   Sphere* spheres;
   Plane* planes;
   Light* lights;
   Color backgroundColor;

   int num_spheres;
   int num_planes;
   int num_lights;
} Scene;

// The raytracer itself, also acting
// as a camera.
typedef struct _Raytracer {
   int width;
   int height;
   int recursionLevels;

   // Initial camera attributes
   Vector3D eye;
   Vector3D at;
   Vector3D up;

   // Basis vectors for working in canonical view space
   Vector3D cameraRight;
   Vector3D cameraUp;
   Vector3D cameraDirec;

   Scene scene;
   Color** image;
   
   // State
   int finished;
   int resizedSelf;
} RayTracer;

// Primary methods used in program
void rayTrace(RayTracer* tracer);
static HitData testObjects(RayTracer* tracer, Ray* ray, float t0, float t1);
static Color rayColor(RayTracer* tracer, Ray* cameraRay, float t0, float t1, int recDepth, int maxRecDepth);
static void computeBasisVecs(RayTracer* tracer);
static Ray computeCameraRay(RayTracer* tracer, int x, int y);
void createDefaultScene(RayTracer* tracer);
void outputImage(RayTracer* tracer);
void onResizeRaytracerScene(RayTracer* tracer, int w, int h);

void initRaytracer(RayTracer* tracer, 
                                        float eyeX, float eyeY, float eyeZ, 
                                        float atX, float atY, float atZ,
                                        int w, int h, int recLevel);


inline static void drawPoint(int x, int y, float r, float g, float b)
{
   glColor3f(r,g,b);
   glVertex3f(x,y,0);
}


#endif
