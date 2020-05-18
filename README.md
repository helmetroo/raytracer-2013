# raytracer-2013 

## What is it? 
This was for a college assignment in my undergrad graphics course. The goal was to write a raytracer in C that could render a simple scene with some objects. The code dates from April 2013. It uses legacy OpenGL to draw the rendered scene to a viewport.

## Build
This was developed on OSX and Linux, and can only be run on those OSes (because of the OpenGL headers).
```bash
make
```

## Running
When you have built the program, run it with the parameters (see below).
```bash
./raytrace eyeX eyeY eyeZ atX atY atZ recDepth
```

You should be able to see a scene with a checkerboard floor, snowman, with a light illuminating the scene.

### Parameters

| Parameter  | Description |
| :--        | :--         |
|                          |
| `eyeX`     | The X position of the camera. |
|                          |
| `eyeY`     | The Y position of the camera. |
|                          |
| `eyeZ`     | The Z position of the camera. |
|                          |
| `atX`     | The X coordinate specifying the direction the camera is looking at. |
|                          |
| `atY`     | The Y coordinate specifying the direction the camera is looking at. |
|                          |
| `atZ`     | The Z coordinate specifying the direction the camera is looking at. |
|                          |
| `recDepth`     | The recursion depth for reflective objects in the scene. |

## Issues
Reflection with a high number of recursion levels tends to have black dots appear at reflections.
I wasn’t exactly sure why this was happening. I tried adding blocks to prevent burned out whites,
but this did nothing. This doesn’t occur if the max recursion level is 0.
