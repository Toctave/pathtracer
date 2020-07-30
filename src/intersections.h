#pragma once
#include "geometry.h"
#include "image_buffer.h"
#include "triangle_mesh.h"

typedef struct Intersect {
    int depth;
    bool hit;
    Vec3 point;
    Vec3 normal;
    float t;
    Vec3 outgoing; // the light's direction, not the algorithm's
    Color outgoing_radiance;
    Material* material;
    Sampler* sampler;
} Intersect;

bool intersect_sphere(Sphere s, Ray r, Intersect* intersect);
bool intersects_sphere(Sphere s, Ray r);

bool intersect_plane(Plane p, Ray r, Intersect* intersect);
bool intersects_plane(Plane p, Ray r);

bool intersect_partial_plane(PartialPlane p, Ray r, Intersect* intersect);
bool intersects_partial_plane(PartialPlane p, Ray r);

bool intersect_triangle(Triangle t, Ray r, Intersect* intersect);
bool intersects_triangle(Triangle t, Ray r);

bool intersect_triangle_mesh(TriangleMesh mesh, Ray r, Intersect* intersect);
bool intersects_triangle_mesh(TriangleMesh mesh, Ray r);
