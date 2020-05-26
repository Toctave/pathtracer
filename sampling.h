#pragma once

#include "geometry.h"

typedef struct Sampler {
    unsigned int seed;
} Sampler;

void sample_unit_square(Sampler* sampler, float* x, float* y, float* pdf);
void sample_unit_disc(Sampler* sampler, float* x, float* y, float* pdf);
Vec3 sample_uniform_hemisphere(Sampler* sampler, float* pdf);
Vec3 sample_cosine_weighted_hemisphere(Sampler* sampler, float* pdf);
uint64_t sample_int(Sampler* sampler);
float rnd(Sampler* sampler);
