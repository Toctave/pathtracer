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
    case GEO_PARTIAL_PLANE:
	did_intersect =
	    intersect_partial_plane(obj.geometry.partial_plane, r, it);
	break;
    }
    if (did_intersect)
	it->material = obj.material;
    return did_intersect;
}

Color trace_ray(Scene* sc, Ray r, int depth, Sampler* sampler) {
    Intersect it;
    it.hit = false;
    it.t = INFINITY;
    it.depth = depth;
    it.sampler = sampler;
    sampler->current_depth = depth;

    for (int i = 0; i < sc->object_count; i++) {
	intersect_object(sc->objects[i], r, &it);
    }

    Color rval;

    if (it.hit) {
	rval = shade(it, sc);
    } else {
	rval = gray(0.0f); // @Todo : color constants
    }
    return rval;
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

/*
void sample_scene(Scene* sc, Camera camera, ImageBuffer* buffer) {
    for (int y = 0; y < buffer->height; y++) {
	for (int x = 0; x < buffer->width; x++) {
	    float dx, dy;
	    sample_unit_square(sc->sampler, &dx, &dy, NULL);
	    float sx = (x + dx) / buffer->width;
	    float sy = 1.0f - (y + dy) / buffer->height;
	    
	    Ray r = camera_ray(camera, sx, sy);

	    add_pixel_sample(buffer, x, y,
			     trace_ray(sc, r, 0, sc->sampler));
	}
    }

}
*/
