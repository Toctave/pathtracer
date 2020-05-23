#pragma once
#include "geometry.h"
#include "image_buffer.h"

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
