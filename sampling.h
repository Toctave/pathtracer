#pragma once

#include "geometry.h"
#include "pcg/pcg_basic.h"
#include "constants.h"
#include <stddef.h> // size_t

typedef struct Sampler {
    pcg32_random_t pcg_state[MAX_DEPTH];
    size_t current_depth;
    float stratified_offsets[MAX_DEPTH][STRATIFIED_RESOLUTION * STRATIFIED_RESOLUTION][2];
    size_t stratified_cell_indices[MAX_DEPTH];
} Sampler;

void create_samplers(Sampler* samplers, size_t n);
void sample_unit_square(Sampler* sampler, float* x, float* y, float* pdf);
void sample_unit_disc(Sampler* sampler, float* x, float* y, float* pdf);
Vec3 sample_uniform_hemisphere(Sampler* sampler, float* pdf);
Vec3 sample_cosine_weighted_hemisphere(Sampler* sampler, float* pdf);
uint32_t sample_int(Sampler* sampler);
float sample_float(Sampler* sampler);
