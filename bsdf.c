#include "bsdf.h"
#include <math.h>
#include <stddef.h>

Vec3 uniform_bsdf_sampler(Sampler* sampler, Vec3 in, float* pdf) {
    return sample_unit_hemisphere(sampler, pdf);
}

Vec3 cosine_bsdf_sampler(Sampler* sampler, Vec3 out, float* pdf) {
    Vec3 sample;
    sample_unit_disc(sampler, &sample.x, &sample.y, NULL);
    
    sample.z = sqrtf(1.0f -
		     sample.x * sample.x -
		     sample.y * sample.y);
    if (isnan(sample.z)) { // can happen because of rounding errors
	sample.z = .0f;
    }
    *pdf = sample.z / M_PI; // p(theta, phi) = cos(theta) / pi
    return sample;
}

Vec3 perfect_reflection_sampler(Sampler* sampler, Vec3 out, float* pdf) {
    *pdf = -1.0f;
    return (Vec3) {out.x, -out.y, out.z};
}

Color lambert_bsdf(void* params, Vec3 in, Vec3 out) {
    Color* col = (Color*) params;

    if (out.z < 0.0f || in.z < 0.0f)
	return (Color) {0.0f};
    else
	return cscale(*col, in.z / M_PI);
}

Color perfect_reflection_bsdf(void* params, Vec3 in, Vec3 out) {
    return (Color) {1.0f, 1.0f, 1.0f}; // should not be called unless in and out are opposite each other
}

Color isotropic_emission(void* params, Vec3 out) {
    Color* col = (Color*) params;

    if (out.z < 0.0f)
	return (Color) {0.0f};
    else
	return cscale(*col, .5f / M_PI);
};
    
