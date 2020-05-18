/**
 * Samuel Sweet
 * CS 5600-1 Assignment 5
 *
 * Represents a plane.
 */

#include "plane.h"

float rayPlaneTest(Ray* ray, Plane* plane)
{
    // Extract surface normal.
    Vector3D surfaceNormal = planeSurfaceNormal(plane);
    float pointDotNormal = dotProd(subtractVec3D(plane->nwPoint, ray->pointOnRay), surfaceNormal);
    float directionDotNormal = dotProd(ray->direction, surfaceNormal);
    
    // Ray parallel to plane, no intersection (avoids division by 0)
    if(directionDotNormal > -0.001f && directionDotNormal < 0.001f)
        return INF;
    
    float intersectT = pointDotNormal / directionDotNormal;
    Vector3D point = vectorAt(*ray, intersectT);
    
    // Throw out the largest coordinate.
    int xIndex = 0;
    int yIndex = 0;
    
    if(fabs(surfaceNormal.z) > fabs(surfaceNormal.x) 
            && fabs(surfaceNormal.z) > fabs(surfaceNormal.y))
    {
        // Project polygon to xy plane.
        xIndex = 0;
        yIndex = 1;
    }
    else if(fabs(surfaceNormal.y) > fabs(surfaceNormal.x))
    {
        // Project polygon to xz plane.
        xIndex = 0;
        yIndex = 2;
    }
    else
    {
        // Project polygon to yz plane.
        xIndex = 1;
        yIndex = 2;
    }
    
    // Test the two triangles
    int goesThroughFirstTriangle = pointTriangleTest(&plane->swPoint, &plane->nwPoint, &plane->nePoint, &point, xIndex, yIndex);
    int goesThroughSecondTriangle = pointTriangleTest(&plane->swPoint, &plane->nePoint, &plane->sePoint, &point, xIndex, yIndex);
    
    // Do the triangle test and return the T if it's in one of the triangles.
    return (goesThroughFirstTriangle | goesThroughSecondTriangle) ? intersectT : INF;
}

static int pointTriangleTest(Vector3D* a, Vector3D* b, Vector3D* c, 
                                        Vector3D* point, int xIndex, int yIndex)
{
    // Strongly based on pseudocode for 
    // http://graphics.stanford.edu/courses/cs348b-98/gg/intersect.html
    float u0 = vecAtIndice(point, xIndex) - vecAtIndice(a, xIndex);
    float v0 = vecAtIndice(point, yIndex) - vecAtIndice(a, yIndex);
    
    float u1 = vecAtIndice(b, xIndex) - vecAtIndice(a, xIndex);
    float u2 = vecAtIndice(c, xIndex) - vecAtIndice(a, xIndex);
    
    float v1 = vecAtIndice(b, yIndex) - vecAtIndice(a, yIndex);
    float v2 = vecAtIndice(c, yIndex) - vecAtIndice(a, yIndex);
    
    float alpha = 0.0f;
    float beta = 0.0f;
    
    // Solves Cramer's rule by computing
    // barycentric coordinates (gamma superfluous).
    if(u1 == 0.0f)
    {
        beta = u0 / u2;
        if(beta >= 0.0f && beta <= 1.0f)
        {
            alpha = (v0 - beta*v2) / v1;
        }
    }
    else
    {
        beta = (v0*u1 - u0*v1) / (v2*u1 - u2*v1);
        if(beta >= 0.0f && beta <= 1.0f)
        {
            alpha = (u0 - beta*u2) / u1;
        }
    }
    
    // Barycentric satisfaction
    return (alpha >= 0.0f && beta >= 0.0f 
            && (alpha + beta) <= 1.0f);
}