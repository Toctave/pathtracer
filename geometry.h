#pragma once
#include <stdbool.h>
#include "image_buffer.h"

#define EPSILON 1e-6

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

typedef struct Intersect {
    bool hit;
    Vec3 point;
    Vec3 normal;
} Intersect;

float norm2(Vec3 v);
float norm(Vec3 v);

Color vec3_to_color(Vec3 v);

void intersect_sphere(Sphere s, Ray r, Intersect* intersect);
