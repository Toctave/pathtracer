#include "scene.h"
#include "shading.h"
#include <math.h>

Color trace_ray(Scene* sc, Ray r) {
    Intersect it;
    it.hit = false;
    it.t = INFINITY;

    for (int i = 0; i < sc->objects.plane_count; i++) {
	intersect_plane(sc->objects.planes[i], r, &it);
    }
    for (int i = 0; i < sc->objects.sphere_count; i++) {
	intersect_sphere(sc->objects.spheres[i], r, &it);
    }

    if (it.hit) {
	return shade(it, sc);
    } else {
	return gray(0.0f); // @Todo : color constants
    }
}

bool free_segment(Scene* sc, Vec3 a, Vec3 b) {
    Intersect it;
    it.hit = false;
    it.t = INFINITY;
    Ray r = {
	.o = a,
	.d = vsub(b, a)
    };
    for (int i = 0; i < sc->objects.plane_count; i++) {
	intersect_plane(sc->objects.planes[i], r, &it);
    }
    for (int i = 0; i < sc->objects.sphere_count; i++) {
	intersect_sphere(sc->objects.spheres[i], r, &it);
    }
    return it.t > 1.0f;
}

void render_scene(Scene* sc, OrthographicCamera camera, ImageBuffer* buffer) {
    for (int y = 0; y < buffer->height; y++) {
	for (int x = 0; x < buffer->width; x++) {
	     float sx = (x + .5f) / buffer->width;
	     float sy = 1.0f - (y + .5f) / buffer->height;

	     Ray r = ortho_camera_ray(camera, sx, sy);

	     buffer->data[y * buffer->width + x] = trace_ray(sc, r);
	}
    }

}
