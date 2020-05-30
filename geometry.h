#pragma once
#include <stdbool.h>

#define EPSILON 1e-6

typedef struct Material Material;
typedef struct Sampler Sampler;

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

typedef struct PartialPlane {
    Vec3 basis[3]; // u, v, normal
    Vec3 origin;
    float min[2];
    float max[2];
} PartialPlane;

PartialPlane create_partial_plane(Vec3 origin, Vec3 u, Vec3 v, float umin, float vmin, float umax, float vmax);

float norm2(Vec3 v);
float norm(Vec3 v);
void vprint(Vec3 v);

static inline Vec3 vadd(Vec3 a, Vec3 b) {
    return (Vec3) {
	a.x + b.x,
	a.y + b.y,
	a.z + b.z
    };
}

static inline Vec3 vneg(Vec3 v) {
    return (Vec3) {
	- v.x,
	- v.y,
	- v.z
    };
}

static inline Vec3 vsub(Vec3 a, Vec3 b) {
    return (Vec3) {
	a.x - b.x,
	a.y - b.y,
	a.z - b.z
    };
}

static inline Vec3 vmul(Vec3 v, float t) {
    return (Vec3) {
	v.x * t,
	v.y * t,
	v.z * t
    };
}

static inline Vec3 vdiv(Vec3 v, float t) {
    return (Vec3) {
	v.x / t,
	v.y / t,
	v.z / t
    };
}

static inline Vec3 normalized(Vec3 v) {
    return vdiv(v, norm(v));
}

static inline Vec3 cross(Vec3 a, Vec3 b) {
    return (Vec3) {
	a.y * b.z - a.z * b.y,
	a.z * b.x - a.x * b.z,
	a.x * b.y - a.y * b.x,
    };
}

static inline Vec3 along_ray(Ray r, float t) {
    return vadd(r.o, vmul(r.d, t));
}

static inline float dot(Vec3 a, Vec3 b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

Vec3 world2basis(Vec3 v, Vec3 x, Vec3 y, Vec3 z);
Vec3 basis2world(Vec3 v, Vec3 x, Vec3 y, Vec3 z);

