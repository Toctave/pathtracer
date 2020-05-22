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

Intersect trace_ray(Scene* sc, Ray r, int depth) {
    Intersect it;
    it.hit = false;
    it.t = INFINITY;
    it.depth = depth;

    for (int i = 0; i < sc->object_count; i++) {
	intersect_object(sc->objects[i], r, &it);
    }

    if (it.hit) {
	it.outgoing_radiance = shade(it, sc);
    } else {
	it.outgoing_radiance = gray(0.0f); // @Todo : color constants
    }
    return it;
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

void sample_scene(Scene* sc, Camera camera, ImageBuffer* buffer) {
    for (int y = 0; y < buffer->height; y++) {
	for (int x = 0; x < buffer->width; x++) {
	    float dx, dy;
	    sample_unit_square(&dx, &dy, NULL);
	    float sx = (x + dx) / buffer->width;
	    float sy = 1.0f - (y + dy) / buffer->height;
	    
	    Ray r = camera_ray(camera, sx, sy);

	    add_pixel_sample(buffer, x, y,
			     trace_ray(sc, r, 0).outgoing_radiance);
	}
    }

}
