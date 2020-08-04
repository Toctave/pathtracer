#include "sampling.h"
#include "globals.h"
#include <math.h>
#include <stdlib.h>

void create_samplers(Sampler* samplers, size_t n) {
    for (size_t i = 0; i < n; i++) {
        for (size_t j = 0; j < MAX_DEPTH; j++) {
            pcg32_srandom_r(&samplers[i].pcg_state[j], i * 4861342 + j * 1243, i * MAX_DEPTH + j);
            samplers[i].stratified_cell_indices[j] = 0;
            // initialize cell offsets
            for (size_t y = 0; y < STRATIFIED_RESOLUTION; y++) {
                for (size_t x = 0; x < STRATIFIED_RESOLUTION; x++) {
                    samplers[i].stratified_offsets[j][y * STRATIFIED_RESOLUTION + x][0] =
                        (float) x / STRATIFIED_RESOLUTION;
                    samplers[i].stratified_offsets[j][y * STRATIFIED_RESOLUTION + x][1] =
                        (float) y / STRATIFIED_RESOLUTION;
                }
            }
            // shuffle them
            samplers[i].current_depth = j;
            for (size_t k = 0; k < STRATIFIED_RESOLUTION * STRATIFIED_RESOLUTION; k++) {
                size_t other = sample_int(&samplers[i]) % (STRATIFIED_RESOLUTION * STRATIFIED_RESOLUTION);
                float tmpx = samplers[i].stratified_offsets[j][k][0];
                float tmpy = samplers[i].stratified_offsets[j][k][1];
                samplers[i].stratified_offsets[j][k][0] =
                    samplers[i].stratified_offsets[j][other][0];
                samplers[i].stratified_offsets[j][k][1] =
                    samplers[i].stratified_offsets[j][other][1];
                samplers[i].stratified_offsets[j][other][0] = tmpx;
                samplers[i].stratified_offsets[j][other][1] = tmpy;
            }
        }
        samplers[i].current_depth = 0;
    }
}

uint32_t sample_int(Sampler* sampler) {
    return pcg32_random_r(&sampler->pcg_state[sampler->current_depth]);
}

float sample_float(Sampler* sampler) {
    uint32_t a = sample_int(sampler);
    a = (0x7f << 23) | (a & 0x007fffff);
    float* b = (float*) &a;
    return *b - 1.0f;
}

void sample_unit_square(Sampler* sampler, float* x, float* y, float* pdf) {
    float dx = sample_float(sampler) / STRATIFIED_RESOLUTION;
    float dy = sample_float(sampler) / STRATIFIED_RESOLUTION;

    size_t cell_index = sampler->stratified_cell_indices[sampler->current_depth];
    *x = sampler->stratified_offsets[sampler->current_depth][cell_index][0] + dx;
    *y = sampler->stratified_offsets[sampler->current_depth][cell_index][1] + dy;

    sampler->stratified_cell_indices[sampler->current_depth] =
        (cell_index + 1) % (STRATIFIED_RESOLUTION * STRATIFIED_RESOLUTION);
    
    if (pdf)
        *pdf = 1.0f;
}

void sample_unit_disc(Sampler* sampler, float* x, float* y, float* pdf) {
    float u, v;
    sample_unit_square(sampler, &u, &v, NULL);
    float theta = 2 * PI * u;
    float r = sqrtf(v);

    *x = r * cosf(theta);
    *y = r * sinf(theta);
    if (pdf)
        *pdf = 1.0f / PI;
}

Vec3 sample_uniform_hemisphere(Sampler* sampler, float* pdf) {
    float cosTheta, v;
    sample_unit_square(sampler, &cosTheta, &v, NULL);
    float sinTheta = sqrtf(1 - cosTheta * cosTheta);
    float cosPhi = cosf(2 * PI * v);
    float sinPhi = sinf(2 * PI * v);

    if (pdf)
        *pdf = .5f / PI;
    return (Vec3) {
        sinTheta * cosPhi,
            sinTheta * sinPhi,
            cosTheta
            };
}

Vec3 sample_cosine_weighted_hemisphere(Sampler* sampler, float* pdf) {
    Vec3 sample;
    sample_unit_disc(sampler, &sample.x, &sample.y, NULL);

    float z2 = 1.0f - sample.x * sample.x - sample.y * sample.y;
    sample.z = sqrtf(z2 > 0.0f ? z2 : 0.0f);
    if (pdf)
        *pdf = sample.z / PI; // p(theta, phi) = cos(theta) / pi
    return sample;
}

