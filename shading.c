#include "shading.h"
#include <math.h>
#include <stdio.h>

#define GAMMA 2.0f

Color shade(Intersect it, Scene* sc) {
    Color rval = gray(.1f);
    for (int i = 0; i < sc->light_count; i++) {
	if (free_segment(sc, it.point, sc->lights[i].position)) {
	    Vec3 tolight = normalized(
		vsub(sc->lights[i].position, it.point)
		);
	    float d = dot(tolight, it.normal);

	    d = d > 0.0f ? d : 0.0f;

	    rval = cadd(rval, gray(pow(d / dot(tolight, tolight), GAMMA)));
	}
    }
    return rval;
}


