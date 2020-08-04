#include "camera.h"
#include "constants.h"
#include <math.h>
#include <stdio.h>

OrthographicCamera create_ortho_camera(Vec3 pos, Vec3 target, float ratio, float scale) {
    OrthographicCamera rval;

    rval.eye = pos;
    rval.basis[2] = normalized(vsub(pos, target));
    rval.basis[0] = normalized(
        cross((Vec3) {0.00123f, -0.000987f, 1.0f}, rval.basis[2])
        );
    rval.basis[1] = cross(rval.basis[2], rval.basis[0]);

    rval.xscale = ratio * scale;
    rval.yscale = scale;

    return rval;
}
    
Ray ortho_camera_ray(OrthographicCamera cam, float sx, float sy) {
    Ray r = {};

    r.o = vadd(
        cam.eye,
        vadd(
            vmul(cam.basis[0], cam.xscale * (-1.0f + 2.0f * sx)),
            vmul(cam.basis[1], cam.yscale * (-1.0f + 2.0f * sy))
            )
        );
    r.d = vneg(cam.basis[2]);

    return r;
}

PerspectiveCamera create_perspective_camera(Vec3 pos, Vec3 target, float ratio, float fov) {
    PerspectiveCamera cam;
    cam.eye = pos;
    cam.depth = 1.0f;

    cam.basis[2] = normalized(vsub(pos, target));
    cam.basis[0] = normalized(
        cross((Vec3) {0.00123f, -0.000987f, 1.0f}, cam.basis[2])
        );
    cam.basis[1] = cross(cam.basis[2], cam.basis[0]);

    cam.xscale = atanf(fov * PI / 360.0f);
    cam.yscale = cam.xscale / ratio;

    return cam;
}

Ray perspective_camera_ray(PerspectiveCamera cam, float sx, float sy) {
    Vec3 basis_ray = normalized(
        (Vec3){ cam.xscale * (-1.0f + 2.0f * sx),
                cam.yscale * (-1.0f + 2.0f * sy),
                -cam.depth
                }
        );
    Ray r = {
        .o = cam.eye,
        .d = basis2world(
            basis_ray,
            cam.basis[0], cam.basis[1], cam.basis[2]
            )
    };
    return r;
};
    

Ray camera_ray(Camera cam, float x, float y) {
    switch (cam.kind) {
    case PERSPECTIVE:
        return perspective_camera_ray(
            cam.camera.perspective, x, y
            );
    case ORTHOGRAPHIC:
        return ortho_camera_ray(
            cam.camera.orthographic, x, y
            );
    default:
        fprintf(stderr, "Unknown camera type\n");
        return (Ray) {};
    }
}

