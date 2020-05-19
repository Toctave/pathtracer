#include "camera.h"

OrthographicCamera create_ortho_camera(Vec3 pos, Vec3 target, float ratio, float scale) {
    OrthographicCamera rval = {
	pos,
	normalized(vsub(target, pos)),
	scale * ratio,
	scale
    };

    return rval;
}
    
Ray ortho_camera_ray(OrthographicCamera cam, float sx, float sy) {
    Ray r = {};

    r.o.x = cam.xscale * (-1.0f + 2.0f * sx);
    r.o.y = cam.yscale * (-1.0f + 2.0f * sy);

    r.o = vadd(r.o, cam.position);

    r.d = cam.direction;

    return r;
}
