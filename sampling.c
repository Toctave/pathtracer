#include "sampling.h"
#include <math.h>
#include <stdlib.h>

unsigned int sample_int(Sampler* sampler) {
    return rand_r(&sampler->seed);
}

float rnd(Sampler* sampler) {
    return (float) rand_r(&sampler->seed) / RAND_MAX;
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

Vec3 sample_uniform_hemisphere(Sampler* sampler, float* pdf) {
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

Vec3 sample_cosine_weighted_hemisphere(Sampler* sampler, float* pdf) {
    Vec3 sample;
    sample_unit_disc(sampler, &sample.x, &sample.y, NULL);

    float z2 = 1.0f - sample.x * sample.x - sample.y * sample.y;
    sample.z = sqrtf(z2 > 0.0f ? z2 : 0.0f);
    if (pdf)
	*pdf = sample.z / M_PI; // p(theta, phi) = cos(theta) / pi
    return sample;
}

