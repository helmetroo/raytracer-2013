/**
 * Samuel Sweet
 * CS 5600-1 Assignment 5
 *
 * Implementation of defined methods in raytracer.h.
 */

#include "raytracer.h"

void initRaytracer(RayTracer* tracer,
        float eyeX, float eyeY, float eyeZ,
        float atX, float atY, float atZ,
        int w, int h, int recLevel)
{
    // Fill out the struct.
    tracer->width = w;
    tracer->height = h;
    tracer->recursionLevels = recLevel;

    Vector3D eye;
    Vector3D at;
    Vector3D up;

    initVector3D(&eye, eyeX, eyeY, eyeZ);
    initVector3D(&at, atX, atY, atZ);
    initVector3D(&up, 0.0f, 1.0f, 0.0f);

    tracer->eye = eye;
    tracer->at = at;
    tracer->up = up;

    // Initialize raster (large, since we work in 32 bits/color channel)
    tracer->image = (Color**) calloc(w, sizeof (Color*));

    for (int col = 0; col < w; ++col)
        tracer->image[col] = (Color*) calloc(h, sizeof (Color));

    tracer->finished = 0;
    tracer->resizedSelf = 0;
}

void rayTrace(RayTracer* tracer)
{
    // Move the camera into canonical space first
    computeBasisVecs(tracer);

    int sceneWidth = tracer->width;
    int sceneHeight = tracer->height;

    // Progress reporting variables
    int pixels_traced = 0;
    int num_pixels = sceneWidth * sceneHeight;
    float percent_change = 0.0f;
    float percent = 0.0f;
    float to_watch = 10.0f;

    // For each pixel
    for(int row_px = 0; row_px < sceneWidth; ++row_px)
    {
        for(int col_px = 0; col_px < sceneHeight; ++col_px)
        {
            // Shoot a ray through that pixel, and extract its color
            Ray cameraRay = computeCameraRay(tracer, row_px, col_px);
            tracer->image[row_px][col_px] = rayColor(tracer, &cameraRay, 0.0f, INF, 0, tracer->recursionLevels);

            // Report progress.
            ++pixels_traced;
            percent_change += (1.0f / (float) num_pixels) * 100.0f;
            percent = ((float) pixels_traced / (float) num_pixels) * 100.0f;

            if(percent_change >= to_watch)
            {
                percent_change = 0.0f;
                printf("%i%% finished\n", (int) percent);
            }
        }
    }

    printf("Finished!\n");
    tracer->finished = 1;
}

// Transforms camera coordinates into uvw coordinate space.
// Done only once.

static void computeBasisVecs(RayTracer* tracer)
{
    Vector3D lookDirec = subtractVec3D(tracer->at, tracer->eye);
    normInPlace(&lookDirec);
    tracer->cameraDirec = lookDirec;

    Vector3D uCrossW = crossProd(tracer->up, tracer->cameraDirec);
    normInPlace(&uCrossW);
    tracer->cameraRight = uCrossW;

    Vector3D wCrossU = crossProd(tracer->cameraDirec, tracer->cameraRight);
    normInPlace(&wCrossU);
    tracer->cameraUp = wCrossU;
}

// Computes an initial starting ray based off some pixel.
// A ray direction is computed that "goes through" that pixel.

static Ray computeCameraRay(RayTracer* tracer, int x, int y)
{
    // Map a screen pixel to an canonical interval [-0.5, 0.5]
    float normX = ((float) x / (float) tracer->width) - 0.5f;
    float normY = -(((float) y / (float) tracer->height) - 0.5f);

    // Multiply against the camera basis vectors to have
    // a vector propagate through that pixel
    Vector3D canonicalRight = scalarMult(normX, tracer->cameraRight);
    Vector3D canonicalUp = scalarMult(normY, tracer->cameraUp);

    // Add up the vectors - multiply the cam direction (forward vector)
    // against some interval which acts as field of view.
    Vector3D canonicalRightAndUpSum = addVec3D(canonicalRight, canonicalUp);
    Vector3D direction = addVec3D(canonicalRightAndUpSum, scalarMult(1.1f, tracer->cameraDirec));
    normInPlace(&direction);

    Ray cameraRay;
    initRayv(&cameraRay, &tracer->eye, &direction);

    return cameraRay;
}

// Iterates through all scene objects and returns data
// regarding the closest.

static HitData testObjects(RayTracer* tracer, Ray* ray, float minT, float maxT)
{
    float intersectT = maxT;

    HitData closestObjData;
    closestObjData.intersectT = INF;

    // Is the sphere closer?
    float sphereT = INF;
    for(int sph_id = 0; sph_id < tracer->scene.num_spheres; ++sph_id)
    {
        Sphere currentSphere = tracer->scene.spheres[sph_id];
        sphereT = raySphereTest(ray, &currentSphere);
        if(sphereT < intersectT && sphereT > minT)
        {
            intersectT = sphereT;
            Vector3D intersectPoint = vectorAt(*ray, intersectT);
            Vector3D sphereNormal = sphereSurfaceNormal(&currentSphere, &intersectPoint);
            initHitData(&closestObjData, sphereNormal, intersectPoint,
                    currentSphere.material, intersectT);
        }
    }

    // Maybe a plane is closer?
    float planeT = INF;
    for(int pla_id = 0; pla_id < tracer->scene.num_planes; ++pla_id)
    {
        Plane currentPlane = tracer->scene.planes[pla_id];
        planeT = rayPlaneTest(ray, &currentPlane);
        if(planeT < intersectT && planeT > minT)
        {
            intersectT = planeT;
            Vector3D intersectPoint = vectorAt(*ray, intersectT);
            initHitData(&closestObjData, planeSurfaceNormal(&currentPlane), intersectPoint,
                    currentPlane.material, intersectT);
        }
    }

    // Return the hit data and the t value we wound up at
    return closestObjData;
}

// Compute shading based on objects that a given ray hits

static Color rayColor(RayTracer* tracer, Ray* cameraRay, float minT, float maxT, int recDepth, int maxRecDepth)
{
    // Figure out if this ray hits an object (return the closest hit).
    HitData hitData = testObjects(tracer, cameraRay, minT, maxT);

    // A hit if the object is not behind the camera or not "at infinity". 
    // Deploy phong, and see if light is not in the
    // way of the object.
    if(hitData.intersectT != INF
            && hitData.intersectT > minT)
    {
        // Base color for the ray is the ambient color of the light and the material.
        Color phongColor = hitData.material.ambient;
        phongColor = addColor(&tracer->scene.lights[0].ambient, &phongColor);

        Vector3D lightVec = subtractVec3D(tracer->scene.lights[0].pos, hitData.intersectPoint);
        Vector3D normLightVec = norm(&lightVec);

        // Begin deploying significant parts of Phong.
        // Specular calculation involves a halfway vector, which is faster to compute than a
        // total reflection vector (no multiplications and dot products).
        Vector3D pointingAtYou = scalarMult(-1.0f, cameraRay->direction);
        Vector3D oppView = norm(&cameraRay->direction);
        Vector3D viewVec = norm(&pointingAtYou);
        Vector3D halfwayVec = addVec3D(norm(&tracer->scene.lights[0].pos), viewVec);
        Vector3D normHalfwayVec = norm(&halfwayVec);

        // If the material is reflective, recursively bounce around scene
        // starting from the intersection point to get the color.
        if(hitData.material.reflection > 0.0f
                && hitData.material.reflection <= 1.0f
                && recDepth < maxRecDepth)
        {
            // Start at ray at the intersection point, and is in the reflection direction
            Vector3D reflection = reflect(hitData.surfaceNormal, oppView);
            Ray reflectRay;
            // Accidentally created transparency by having view vec used down here
            initRayv(&reflectRay, &hitData.intersectPoint, &reflection);

            Color reflectionColor = rayColor(tracer, &reflectRay, EPSILON, INF, recDepth + 1, maxRecDepth);
            Color modReflectionColor = scalarMultColor(hitData.material.reflection, &reflectionColor);
            phongColor = addColor(&phongColor, &modReflectionColor);
        }

        // For shadows, we calculate a new ray starting a smidge off the intersection point.
        Ray lightRay;
        initRayv(&lightRay, &hitData.intersectPoint, &normLightVec);
        HitData lightHitData = testObjects(tracer, &lightRay, EPSILON, INF);

        if(lightHitData.intersectT == INF)
        {
            // Diffuse shading (and shadow handling)
            float diffuseCoeff = dotProd(normLightVec, hitData.surfaceNormal);
            float finalDiffuseCoeff = (diffuseCoeff > 0.0f) ? diffuseCoeff : 0.0f;

            Color diffuseColor;

            // Is there a dynamic texture? Use it if defined
            if(hitData.material.function != NULL)
            {
                Color calcDiffuseColor = hitData.material.function(hitData.intersectPoint.x, hitData.intersectPoint.z);
                diffuseColor = scalarMultColor(finalDiffuseCoeff, &calcDiffuseColor);
            } 
            else
            {
                diffuseColor = scalarMultColor(finalDiffuseCoeff, &hitData.material.diffuse);
            }

            phongColor = addColor(&phongColor, &diffuseColor);

            float reflectCoeff = dotProd(normHalfwayVec, hitData.surfaceNormal);
            float specPower = pow(reflectCoeff, hitData.material.specularCoeff);
            Color specularColor = scalarMultColor(specPower, &hitData.material.specular);
            phongColor = addColor(&phongColor, &specularColor);
        }

        return phongColor;
    } 
    else
    {
        // No hits, so we just see the background.
        return tracer->scene.backgroundColor;
    }
}

// Creates a default scene.

void createDefaultScene(RayTracer* tracer)
{
    // Allocate memory for all scene objects
    tracer->scene.num_spheres = 4;
    tracer->scene.num_planes = 5;
    tracer->scene.num_lights = 1;

    tracer->scene.spheres = (Sphere*) calloc(tracer->scene.num_spheres, sizeof (Sphere));
    tracer->scene.planes = (Plane*) calloc(tracer->scene.num_planes, sizeof (Plane));
    tracer->scene.lights = (Light*) calloc(tracer->scene.num_lights, sizeof (Light));

    // Define our spheres in the scene.
    Sphere snowmanSphere_bottom;
    Material snowmanSphere_bottomMaterial;
    setAmbient(&snowmanSphere_bottomMaterial, 0.1f, 0.25f, 0.1f, 1.0f);
    setDiffuse(&snowmanSphere_bottomMaterial, 0.4f, 0.75f, 0.25f, 1.0f);
    setSpecular(&snowmanSphere_bottomMaterial, 1.0f, 1.0f, 1.0f, 1.0f);
    setSpecularCoeff(&snowmanSphere_bottomMaterial, 10.0f);
    setReflection(&snowmanSphere_bottomMaterial, 0.0f);
    initSphere(&snowmanSphere_bottom, 0.7f, -0.55f, -1.7f, 0.55f, &snowmanSphere_bottomMaterial);

    Sphere snowmanSphere_middle;
    Material snowmanSphere_middleMaterial;
    setAmbient(&snowmanSphere_middleMaterial, 0.4f, 0.15f, 0.1f, 1.0f);
    setDiffuse(&snowmanSphere_middleMaterial, 0.4f, 0.55f, 0.15f, 1.0f);
    setSpecular(&snowmanSphere_middleMaterial, 1.0f, 1.0f, 1.0f, 1.0f);
    setSpecularCoeff(&snowmanSphere_middleMaterial, 40.0f);
    setReflection(&snowmanSphere_middleMaterial, 0.1f);
    initSphere(&snowmanSphere_middle, 0.7f, 0.2f, -1.7f, 0.45f, &snowmanSphere_middleMaterial);

    Sphere snowmanSphere_top;
    Material snowmanSphere_topMaterial;
    setAmbient(&snowmanSphere_topMaterial, 0.35f, 0.15f, 0.6f, 1.0f);
    setDiffuse(&snowmanSphere_topMaterial, 0.4f, 0.25f, 0.15f, 1.0f);
    setSpecular(&snowmanSphere_topMaterial, 1.0f, 1.0f, 1.0f, 1.0f);
    setSpecularCoeff(&snowmanSphere_topMaterial, 170.0f);
    setReflection(&snowmanSphere_topMaterial, 0.0f);
    initSphere(&snowmanSphere_top, 0.7f, 0.8f, -1.7f, 0.35f, &snowmanSphere_topMaterial);

    // Define our spheres in the scene.
    Sphere floorSphere;
    Material floorSphereMaterial;
    setAmbient(&floorSphereMaterial, 0.0f, 0.0f, 0.0f, 1.0f);
    setDiffuse(&floorSphereMaterial, 0.2f, 0.35f, 0.65f, 1.0f);
    setSpecular(&floorSphereMaterial, 1.0f, 1.0f, 1.0f, 1.0f);
    setSpecularCoeff(&floorSphereMaterial, 100.0f);
    setReflection(&floorSphereMaterial, 1.0f);
    initSphere(&floorSphere, -0.9f, -0.55f, -1.4f, 0.55f, &floorSphereMaterial);

    tracer->scene.spheres[0] = snowmanSphere_bottom;
    tracer->scene.spheres[1] = snowmanSphere_middle;
    tracer->scene.spheres[2] = snowmanSphere_top;
    tracer->scene.spheres[3] = floorSphere;


    // Define planes in scene (cubes, floor)
    // Writing a function pointer and relating indices to cube vertex positions
    // would have been better?
    Material brassMaterial;
    setAmbient(&brassMaterial, 0.329412f, 0.223529f, 0.027451f, 1.0f);
    setDiffuse(&brassMaterial, 0.780392f, 0.568627f, 0.113725f, 1.0f);
    setSpecular(&brassMaterial, 0.992157f, 0.941176f, 0.807843f, 1.0f);
    setSpecularCoeff(&brassMaterial, 27.8974f);
    setReflection(&brassMaterial, 0.2f);

    Plane cubeFrontPlane;
    initPlanefv(&cubeFrontPlane, 0.0f, -1.0f, -3.0f,
                                0.0f, 0.0f, -3.0f,
                                -1.0f, 0.0f, -3.0f,
                                -1.0f, -1.0f, -3.0f,
                                &brassMaterial);

    Plane cubeTopPlane;
    initPlanefv(&cubeTopPlane, 0.0f, 0.0f, -3.0f,
                                0.0f, 0.0f, -4.0f,
                                -1.0f, 0.0f, -4.0f,
                                -1.0f, 0.0f, -3.0f,
                                &brassMaterial);

    Plane cubeLeftPlane;
    initPlanefv(&cubeLeftPlane, 0.0f, -1.0f, -4.0f,
                                0.0f, 0.0f, -4.0f,
                                0.0f, 0.0f, -3.0f,
                                0.0f, -1.0f, -3.0f,
                                &brassMaterial);

    Plane cubeRightPlane;
    initPlanefv(&cubeRightPlane, -1.0f, -1.0f, -4.0f,
                                    -1.0f, 0.0f, -4.0f,
                                    -1.0f, 0.0f, -3.0f,
                                    -1.0f, -1.0f, -3.0f,
                                    &brassMaterial);

    tracer->scene.planes[0] = cubeFrontPlane;
    tracer->scene.planes[1] = cubeTopPlane;
    tracer->scene.planes[2] = cubeLeftPlane;
    tracer->scene.planes[3] = cubeRightPlane;


    Plane groundPlane;
    Material checkerboardMaterial;
    setAmbient(&checkerboardMaterial, 0.35f, 0.15f, 0.6f, 1.0f);
    setSpecular(&checkerboardMaterial, 1.0f, 1.0f, 1.0f, 1.0f);
    setSpecularCoeff(&checkerboardMaterial, 10.0f);
    setReflection(&checkerboardMaterial, 0.15f);
    setFunction(&checkerboardMaterial, &checkerboardMap);
    initPlanefv(&groundPlane, 5.0f, -1.0f, 0.0f,
                                    5.0f, -1.0f, -10.0f,
                                    -5.0f, -1.0f, -10.0f,
                                    -5.0f, -1.0f, 0.0f,
                                    &checkerboardMaterial);


    tracer->scene.planes[4] = groundPlane;

    // Define lights.
    initLightff(&tracer->scene.lights[0], 0.0f, 0.09f, 0.05f, 1.0f, 4.7f, 12.5f, 12.0f);

    // Periwinkle.
    initColor(&tracer->scene.backgroundColor, 0.2384f, 0.3166f, 0.6538f, 1.0f);
}

void onResizeRaytracerScene(RayTracer* tracer, int w, int h)
{
    tracer->image = (Color**) calloc(w, sizeof (Color*));

    for (int col = 0; col < w; ++col)
        tracer->image[col] = (Color*) calloc(h, sizeof (Color));

    tracer->width = w;
    tracer->height = h;
    tracer->finished = 0;

    rayTrace(tracer);
}

void outputImage(RayTracer* tracer)
{
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    if (!tracer->finished)
    {
        // Draw nothing if we don't have a finished image.
        glBegin(GL_POINTS);
        glEnd();
        glFlush();
        glutSwapBuffers();
        return;
    }

    // The image is finished and ready to be drawn
    glBegin(GL_POINTS);
    for (int col = 0; col < tracer->width; ++col)
    {
        for (int row = 0; row < tracer->height; ++row)
        {
            // Reminder that I have OpenGL set up where low y values are
            // at the bottom of the viewport, so y is inverted.
            drawPoint(row, tracer->height - col,
                        tracer->image[row][col].red,
                        tracer->image[row][col].green,
                        tracer->image[row][col].blue);
        }
    }
    glEnd();
    glFlush();

    glutSwapBuffers();
}