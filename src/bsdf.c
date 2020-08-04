#include "bsdf.h"
#include "globals.h"
#include <math.h>
#include <stddef.h>

Vec3 uniform_bsdf_sampler(void* params, Sampler* sampler, Vec3 in, float* pdf) {
    return sample_uniform_hemisphere(sampler, pdf);
}

Vec3 cosine_weighted_bsdf_sampler(void* params, Sampler* sampler, Vec3 out, float* pdf) {
    return sample_cosine_weighted_hemisphere(sampler, pdf);
}

Vec3 perfect_reflection_sampler(void* params, Sampler* sampler, Vec3 out, float* pdf) {
    *pdf = -1.0f;
    return (Vec3) {out.x, -out.y, out.z};
}

Vec3 perfect_refraction_sampler(void* params, Sampler* sampler, Vec3 out, float* pdf) {
    return (Vec3) {}; // @TODO(octave)
}

Color lambert_bsdf(void* params, Vec3 in, Vec3 out) {
    Color* col = (Color*) params;

    if (out.z < 0.0f || in.z < 0.0f)
        return (Color) {0.0f};
    else
        return cscale(*col, 1.0f / PI);
}

Color perfect_reflection_bsdf(void* params, Vec3 in, Vec3 out) {
    float factor = 1.0f / out.z;
    return (Color) {factor, factor, factor}; // should not be called unless in and out are opposite each other
}

Color isotropic_emission(void* params, Vec3 out) {
    Color* col = (Color*) params;

    if (out.z < 0.0f)
        return (Color) {0.0f};
    else
        return cscale(*col, .5f / PI);
};

