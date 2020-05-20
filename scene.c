#include "scene.h"
#include "shading.h"
#include "sampling.h"
#include <math.h>

// @Speed : avoid last-minute decision
bool intersect_object(Object obj, Ray r, Intersect* it) {
    bool did_intersect;
    switch (obj.kind) {
    case GEO_SPHERE:
	did_intersect =
	    intersect_sphere(obj.geometry.sphere, r, it);
	break;
    case GEO_PLANE:
	did_intersect =
	    intersect_plane(obj.geometry.plane, r, it);
	break;
    }
    if (did_intersect)
	it->material = obj.material;
    return did_intersect;
}

Color trace_ray(Scene* sc, Ray r) {
    Intersect it;
    it.hit = false;
    it.t = INFINITY;

    for (int i = 0; i < sc->object_count; i++) {
	intersect_object(sc->objects[i], r, &it);
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
    for (int i = 0; i < sc->object_count; i++) {
	intersect_object(sc->objects[i], r, &it);
    }
    return it.t * it.t > norm2(r.d);
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
