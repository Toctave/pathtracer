#pragma once
#include "geometry.h"

typedef enum { PERSPECTIVE, ORTHOGRAPHIC } CameraKind;

typedef struct OrthographicCamera {
    Vec3 eye;
    Vec3 basis[3];
    float xscale;
    float yscale;
} OrthographicCamera;

typedef struct PerspectiveCamera {
    Vec3 eye;
    Vec3 basis[3];
    float xscale;
    float yscale;
    float depth;
} PerspectiveCamera;

typedef struct Camera {
    CameraKind kind;
    union {
        PerspectiveCamera perspective;
        OrthographicCamera orthographic;
    } camera;
} Camera;

OrthographicCamera create_ortho_camera(Vec3 pos, Vec3 target, float ratio, float scale);
PerspectiveCamera create_perspective_camera(Vec3 pos, Vec3 target, float ratio, float fov);

Ray ortho_camera_ray(OrthographicCamera cam, float x, float y);
Ray perspective_camera_ray(PerspectiveCamera cam, float x, float y);

Ray camera_ray(Camera cam, float x, float y);

