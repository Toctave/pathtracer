#pragma once
#include "geometry.h"
#include "image_buffer.h"
#include "scene.h"

typedef struct Light {
    Vec3 position;
    Color color;
    float intensity;
} Light;

Color shade(Intersect it, Scene* sc);

