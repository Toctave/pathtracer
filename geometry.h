#pragma once
#include <stdbool.h>
#include "image_buffer.h"

#define EPSILON 1e-6

typedef struct Material Material;

typedef struct Vec3 {
    float x;
    float y;
    float z;
} Vec3;

typedef struct Ray {
    Vec3 o;
    Vec3 d;
} Ray;

typedef struct Sphere {
    Vec3 center;
    float radius;
} Sphere;

typedef struct Plane {
    Vec3 normal;
    float distance;
} Plane;

typedef struct Intersect {
    int depth;
    bool hit;
    Vec3 point;
    Vec3 normal;
    float t;
    Vec3 outgoing; // the light's direction, not the algorithm's
    Color outgoing_radiance;
    Material* material;
} Intersect;

float norm2(Vec3 v);
float norm(Vec3 v);
Vec3 normalized(Vec3 v);
void vprint(Vec3 v);

Vec3 vneg(Vec3 v);
Vec3 vsub(Vec3 a, Vec3 b);
Vec3 vadd(Vec3 a, Vec3 b);
Vec3 vmul(Vec3 a, float t);
Vec3 along_ray(Ray r, float t);
Vec3 vdiv(Vec3 a, float t);
float dot(Vec3 a, Vec3 b);
Vec3 cross(Vec3 a, Vec3 b);

Vec3 world2basis(Vec3 v, Vec3 x, Vec3 y, Vec3 z);
Vec3 basis2world(Vec3 v, Vec3 x, Vec3 y, Vec3 z);

Color vec3_to_color(Vec3 v);

bool intersect_sphere(Sphere s, Ray r, Intersect* intersect);
bool intersects_sphere(Sphere s, Ray r);

bool intersect_plane(Plane p, Ray r, Intersect* intersect);
bool intersects_plane(Plane p, Ray r);
