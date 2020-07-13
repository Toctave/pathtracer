#include "shading.h"
#include "bsdf.h"
#include "scene.h"
#include "debug.h"
#include "constants.h"
#include <math.h>
#include <stdio.h>

Color shade(Intersect it, Scene* sc) {
    // @Todo : think about the right shifting amount
    Vec3 surface_point = vadd(it.point,
			      vmul(it.normal, 2 * EPSILON));
    Vec3 u = cross(it.outgoing, it.normal);
    if (norm2(u) < EPSILON) {
	u = cross(it.outgoing, (Vec3) {0.004f, 0.0212f, 0.9998f});
    }
    u = normalized(u);

    Vec3 v = cross(it.normal, u);

    // EMITTED LIGHT
    Vec3 local_out = world2basis(it.outgoing, u, v, it.normal);
    Color rval = {0.0f};
    if (it.material->bsdf->emitted) {
	rval = it.material->bsdf->emitted(it.material->params, local_out);
    }

    if (it.material->bsdf->f) {
	// INDIRECT LIGHTING
	if (it.depth < MAX_DEPTH) {
	    float pdf;
	    Vec3 bounce_sample = it.material->bsdf->sampler(it.sampler, local_out, &pdf);
	    Ray bounce = {
		.o = surface_point,
		.d = basis2world(bounce_sample, u, v, it.normal)
	    };

	    ASSERT(FEQ(norm2(local_out), 1.0f));
	    ASSERT(FEQ(norm2(u), 1.0f));
	    ASSERT(FEQ(norm2(v), 1.0f));
	    ASSERT(FEQ(norm2(bounce.d), 1.0f));
	    
	    Color incoming_radiance = trace_ray(sc, bounce, it.depth + 1, it.sampler);
	    Color f = it.material->bsdf->f(it.material->params,
					   bounce_sample, local_out);
	    float inv_pdf = pdf > EPSILON ? (1.0f / pdf) : 1.0f;
	    rval = cscale(
		cmul(incoming_radiance, f),
		inv_pdf * bounce_sample.z);
	}
    }


    return rval;
}


