#pragma once

#include "geometry.h"
#include "camera.h"
#include "bsdf.h"
#include <stddef.h> // size_t

struct Light;
typedef enum { GEO_PLANE, GEO_SPHERE } GeometryKind;

typedef struct Object {
    GeometryKind kind;
    union {
	Sphere sphere;
	Plane plane;
    } geometry;
    Material* material;
} Object;

typedef struct Scene {
    size_t object_count;
    Object* objects;
    
    size_t light_count;
    struct Light* lights;
} Scene;

void sample_scene(Scene* sc, OrthographicCamera camera, ImageBuffer* buffer);
bool free_segment(Scene* sc, Vec3 a, Vec3 b);
Intersect trace_ray(Scene* sc, Ray r, int depth);
