#pragma once

#include "geometry.h"
#include "image_buffer.h"
#include "sampling.h"

// BSDF : (void* params, Vec3 in, Vec3 out) -> Color
typedef Color (*BSDFTransmission)(void*, Vec3, Vec3);
// BSDFSampler : (Vec3 out, float* pdf) -> Vec3 out
typedef Vec3 (*BSDFSampler)(Vec3, float*);

typedef Color (*BSDFEmission)(void*, Vec3);

typedef struct BSDF {
    BSDFEmission emitted;
    BSDFTransmission f;
    BSDFSampler sampler;
} BSDF;

typedef struct Material {
    BSDF* bsdf;
    void* params;
} Material;

// Samplers
Vec3 uniform_bsdf_sampler(Vec3 out, float* pdf);
Vec3 cosine_bsdf_sampler(Vec3 out, float* pdf);

// Transmission functions
Color lambert_bsdf(void* params, Vec3 in, Vec3 out);
Color perfect_reflection(void* params, Vec3 in, Vec3 out);

// Emission functions
Color isotropic_emission(void* params, Vec3 out);
