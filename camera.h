#pragma once
#include "geometry.h"

typedef struct OrthographicCamera {
    Vec3 position;
    Vec3 direction;
    float xscale;
    float yscale;
} OrthographicCamera;

OrthographicCamera create_ortho_camera(Vec3 pos, Vec3 target, float ratio, float scale);
Ray ortho_camera_ray(OrthographicCamera cam, float x, float y);

