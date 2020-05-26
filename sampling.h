#pragma once

#include "geometry.h"
#include "pcg/pcg_basic.h"
#include <stddef.h> // size_t

#define STRATIFIED_RESOLUTION 4

typedef struct Sampler {
    pcg32_random_t pcg_state;
} Sampler;

void create_samplers(Sampler* samplers, size_t n);
void sample_unit_square(Sampler* sampler, float* x, float* y, float* pdf);
void sample_unit_disc(Sampler* sampler, float* x, float* y, float* pdf);
Vec3 sample_uniform_hemisphere(Sampler* sampler, float* pdf);
Vec3 sample_cosine_weighted_hemisphere(Sampler* sampler, float* pdf);
uint64_t sample_int(Sampler* sampler);
float rnd(Sampler* sampler);
