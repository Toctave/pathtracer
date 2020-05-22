#include "shading.h"
#include "bsdf.h"
#include "scene.h"
#include <math.h>
#include <stdio.h>

#define MAX_DEPTH 3

Color shade(Intersect it, Scene* sc) {
    // @Todo : think about the right shifting amount
    Vec3 surface_point = vadd(it.point,
			      vmul(it.normal, 2 * EPSILON));
    Vec3 u = normalized(cross(it.outgoing, it.normal));
    Vec3 v = cross(it.normal, u);

    // EMITTED LIGHT
    Vec3 local_out = world2basis(it.outgoing, u, v, it.normal);
    Color rval = {0.0f};
    if (it.material->bsdf->emitted) {
	rval = it.material->bsdf->emitted(it.material->params, local_out);
    }

    if (it.material->bsdf->f) {
	// DIRECT LIGHTING
	for (int i = 0; i < sc->light_count && false; i++) {
	    Light l = sc->lights[i];
	    if (free_segment(sc, surface_point, l.position)) {
		Vec3 tolight = normalized(
		    vsub(l.position, it.point)
		    );

		Vec3 in = world2basis(tolight, u, v, it.normal);

		float light_attenuation = 1.0f / dot(tolight, tolight);

		// @Speed : too many dereferences
		Color f =
		    it.material->bsdf->f(it.material->params, in, local_out);
		rval = cadd(rval,
			    cscale(cmul(f, l.color),
				   light_attenuation * l.intensity
				)
		    );
	    }
	}
	// INDIRECT LIGHTING
	if (it.depth < MAX_DEPTH) {
	    float pdf;
	    Vec3 bounce_sample = it.material->bsdf->sampler(local_out, &pdf);
	    Ray bounce = {
		.o = surface_point,
		.d = basis2world(bounce_sample, u, v, it.normal)
	    };

	    Intersect bounce_it = trace_ray(sc, bounce, it.depth + 1);
	    Color f = it.material->bsdf->f(it.material->params,
					   bounce_sample, local_out);
	    Color dc = cmul(
		bounce_it.outgoing_radiance,
		f);
	    rval = cadd(rval,
			cscale(dc, 1.0f / pdf));
	}
    }

    return rval;
}


