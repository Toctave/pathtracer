#include "bsdf.h"
#include <math.h>

Vec3 uniform_bsdf_sampler(Vec3 in, float* pdf) {
    return sample_unit_hemisphere(pdf);
}

Color lambert_bsdf(void* params, Vec3 in, Vec3 out) {
    Color* col = (Color*) params;

    if (out.z < 0.0f || in.z < 0.0f)
	return (Color) {0.0f};
    else
	return cscale(*col, in.z / M_PI);
}

Color isotropic_emission(void* params, Vec3 out) {
    Color* col = (Color*) params;

    if (out.z < 0.0f)
	return (Color) {0.0f};
    else
	return cscale(*col, .5f / M_PI);
};
    
