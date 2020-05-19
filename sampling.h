#pragma once

#include "geometry.h"

void sample_unit_square(float* x, float* y, float* pdf);
void sample_unit_disc(float* x, float* y, float* pdf);
Vec3 sample_unit_hemisphere(float* pdf);

