#include "camera.h"

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
