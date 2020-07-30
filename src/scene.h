#pragma once

#include "geometry.h"
#include "camera.h"
#include "bsdf.h"
#include "sampling.h"
#include "intersections.h"
#include <stddef.h> // size_t

struct Light;
typedef enum { GEO_PLANE, GEO_PARTIAL_PLANE, GEO_SPHERE, GEO_TRIANGLE, GEO_TRIANGLE_MESH } GeometryKind;

typedef struct Object {
    GeometryKind kind;
    union {
	Sphere sphere;
	Plane plane;
	PartialPlane partial_plane;
	Triangle triangle;
	TriangleMesh triangle_mesh;
    } geometry;
    Material* material;
} Object;

typedef struct Scene {
    size_t object_count;
    Object* objects;
    
    size_t light_count;
    struct Light* lights;
} Scene;

void sample_scene(Scene* sc, Camera camera, ImageBuffer* buffer);
bool free_segment(Scene* sc, Vec3 a, Vec3 b);
Color trace_ray(Scene* sc, Ray r, int depth, Sampler* s);
