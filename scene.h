#pragma once

#include "geometry.h"
#include "camera.h"
#include <stddef.h> // size_t

struct Light;

typedef struct Scene {
    struct {
	size_t sphere_count;
	Sphere* spheres;
	size_t plane_count;
	Plane* planes;
    } objects;
    size_t light_count;
    struct Light* lights;
} Scene;

void render_scene(Scene* sc, OrthographicCamera camera, ImageBuffer* buffer);
bool free_segment(Scene* sc, Vec3 a, Vec3 b);
