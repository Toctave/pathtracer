#include "sampling.h"
#include <math.h>
#include <stdlib.h>

float rnd() {
    return (float) rand() / RAND_MAX;
}

void sample_unit_square(float* x, float* y, float* pdf) {
    *x = rnd();
    *y = rnd();
    if (pdf)
	*pdf = 1.0f;
}

void sample_unit_disc(float* x, float* y, float* pdf) {
    float u, v;
    sample_unit_square(&u, &v, NULL);
    float theta = 2 * M_PI * u;
    float r = sqrtf(v);

    *x = r * cosf(theta);
    *y = r * sinf(theta);
    if (pdf)
	*pdf = 1.0f / M_PI;
}

Vec3 sample_unit_hemisphere(float* pdf) {
    float cosTheta, v;
    sample_unit_square(&cosTheta, &v, NULL);
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
