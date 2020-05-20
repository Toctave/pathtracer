#pragma once

#include "geometry.h"
#include "image_buffer.h"
#include "sampling.h"

// BSDF : (void* params, Vec3 in, Vec3 out) -> Color
typedef Color (*BSDFFunction)(void*, Vec3, Vec3);
// BSDFSampler : (Vec3 out, float* pdf) -> Vec3 out
typedef Vec3 (*BSDFSampler)(Vec3, float*);

typedef struct BSDF {
    BSDFFunction f;
    BSDFSampler sampler;
} BSDF;

typedef struct Material {
    BSDF* bsdf;
    void* params;
} Material;

Vec3 uniform_bsdf_sampler(Vec3 out, float* pdf);

Color lambert_bsdf(void* params, Vec3 in, Vec3 out);


