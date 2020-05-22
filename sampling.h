#pragma once

#include "geometry.h"

typedef struct Sampler {
    unsigned int seed;
} Sampler;

void sample_unit_square(Sampler* sampler, float* x, float* y, float* pdf);
void sample_unit_disc(Sampler* sampler, float* x, float* y, float* pdf);
Vec3 sample_unit_hemisphere(Sampler* sampler, float* pdf);
unsigned int sample_int(Sampler* sampler);
float rnd(Sampler* sampler);
