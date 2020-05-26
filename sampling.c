#include "sampling.h"
#include <math.h>
#include <stdlib.h>

void create_samplers(Sampler* samplers, size_t n) {
    for (int i = 0; i < n; i++) {
	pcg32_srandom_r(&samplers[i].pcg_state, i * 4861342, i);
    }
}

unsigned int sample_int(Sampler* sampler) {
    return pcg32_random_r(&sampler->pcg_state);
}

float rnd(Sampler* sampler) {
    return (float) sample_int(sampler) / RAND_MAX;
}

void sample_unit_square(Sampler* sampler, float* x, float* y, float* pdf) {
    *x = rnd(sampler);
    *y = rnd(sampler);
    if (pdf)
	*pdf = 1.0f;
}

void sample_unit_disc(Sampler* sampler, float* x, float* y, float* pdf) {
    float u, v;
    sample_unit_square(sampler, &u, &v, NULL);
    float theta = 2 * M_PI * u;
    float r = sqrtf(v);

    *x = r * cosf(theta);
    *y = r * sinf(theta);
    if (pdf)
	*pdf = 1.0f / M_PI;
}

Vec3 sample_unit_hemisphere(Sampler* sampler, float* pdf) {
    float cosTheta, v;
    sample_unit_square(sampler, &cosTheta, &v, NULL);
    float sinTheta = sqrtf(1 - cosTheta * cosTheta);
    float cosPhi = cosf(2 * M_PI * v);
    float sinPhi = sinf(2 * M_PI * v);

    if (pdf)
	*pdf = .5f / M_PI;
    return (Vec3) {
	sinTheta * cosPhi,
	sinTheta * sinPhi,
	cosTheta
    };
}
