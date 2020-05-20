#include "shading.h"
#include "bsdf.h"
#include <math.h>
#include <stdio.h>

#define GAMMA 2.0f

Color shade(Intersect it, Scene* sc) {
    BSDF lambert = {
	.f = lambert_bsdf,
	.sampler = uniform_bsdf_sampler,
    };
    Color white = gray(1.0f);
    Color rval = {.0f, .0f, .0f};

    // @Todo : think about the right shifting amount
    Vec3 surface_point = vadd(it.point,
			      vmul(it.normal, 2 * EPSILON));
    Vec3 u = normalized(cross(it.outgoing, it.normal));
    Vec3 v = cross(it.normal, u);

    // DIRECT LIGHTING
    for (int i = 0; i < sc->light_count; i++) {
	Light l = sc->lights[i];
	if (free_segment(sc, surface_point, l.position)) {
	    Vec3 tolight = normalized(
		vsub(l.position, it.point)
		);

	    Vec3 in = in_basis(tolight, u, v, it.normal);
	    Vec3 out = in_basis(it.outgoing, u, v, it.normal);

	    float light_attenuation = 1.0f / dot(tolight, tolight);

	    Color f =
		it.material->bsdf->f(it.material->params, in, out);
	    rval = cadd(rval,
			cscale(cmul(f, l.color),
			       light_attenuation * l.intensity
			    )
		);
	}
    }

    // INDIRECT LIGHTING
    // @Todo !
    return rval;
}


