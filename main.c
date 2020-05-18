/**
 * Samuel Sweet
 * CS 5600-1 Assignment 5
 *
 * The main entry point for the program.
 */

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

/* Standard libraries */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "raytracer.h"

// Global raytracer
static RayTracer tracer;
static int mainWindow = 1;

// Event handlers for GLUT
static void onKeyDown(unsigned char key, int x, int y);
static void onWindowResize(int w, int h);
static void onUpdate(void);
static void outputRaytracerImage(void);

int main(int argc, char* argv[])
{
// Remind user to provide 8 parameters (including our executable)
   if(argc != 8)
   {
      printf("Incorrect parameters provided.\nUsage: raytrace eyeX eyeY eyeZ atX atY atZ recDepth\n");
      exit(1);
   }
   
   // Grab our command line parameters and initialize the raytracer.
   float eyeX = (float) atof(argv[1]);
   float eyeY = (float) atof(argv[2]);
   float eyeZ = (float) atof(argv[3]);
   
   float atX = (float) atof(argv[4]);
   float atY = (float) atof(argv[5]);
   float atZ = (float) atof(argv[6]);
   
   int recLevels = atoi(argv[7]);
   
   // Begin building
   initRaytracer(&tracer, eyeX, eyeY, eyeZ, atX, atY, atZ, 512, 512, recLevels);
   createDefaultScene(&tracer);
   rayTrace(&tracer);
   
   // Raytrace.
   
// Init X windows
   glutInit(&argc, argv);

//
// Init GLUT with specified features
//
   glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
   glutInitWindowSize(512, 512);
   glutInitWindowPosition(100, 100);
   mainWindow = glutCreateWindow("Raytracer");
   glPointSize(1);

   glutDisplayFunc(outputRaytracerImage);
   glutKeyboardFunc(onKeyDown);
   glutReshapeFunc(onWindowResize);
   glutIdleFunc(onUpdate);

// 2D projection for the window.
   glMatrixMode(GL_PROJECTION);
   glOrtho(0, 512, 0, 512, -100, 10000);
   glMatrixMode(GL_MODELVIEW);
   
// Put GLUT in its update loop
   glutMainLoop();
   return 0;
}

// When a key is pressed.
static void onKeyDown(unsigned char key, int x, int y)
{
   switch(key)
   {
      // Quit on press of Q.
      case 'q':
      case 'Q':
	 exit(0);
	 break;
   }
   
   glutPostRedisplay();
}

// If the window resizes, recompute the image (experimental).
static void onWindowResize(int w, int h)
{
   glViewport(0,0,w,h);
   glMatrixMode(GL_PROJECTION);

   glLoadIdentity();
   glOrtho(0,w,0,h,-100,10000);
   glMatrixMode(GL_MODELVIEW);

   // Prevents raytracing again since this
   // method is called initially.
   if(!tracer.resizedSelf)
       tracer.resizedSelf = 1;
   else
       onResizeRaytracerScene(&tracer, w, h);

   // Redraw scene?
   glutPostRedisplay();
}

// Redraws the scene.
static void onUpdate(void)
{
   if(glutGetWindow() != mainWindow)
      glutSetWindow(mainWindow);

   glutPostRedisplay();
}

// Tells the raytracer to output the image.
static void outputRaytracerImage(void)
{
    outputImage(&tracer);
}
