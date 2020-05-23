#include "geometry.h"
#include "image_buffer.h"
#include <math.h>
#include <stdio.h>

float norm2(Vec3 v) {
    return v.x * v.x + v.y * v.y + v.z * v.z;
}

float norm(Vec3 v) {
    return sqrtf(norm2(v));
}

Vec3 world2basis(Vec3 v, Vec3 x, Vec3 y, Vec3 z) {
    return (Vec3) {
	dot(v, x),
	dot(v, y),
	dot(v, z)
    };
}

Vec3 basis2world(Vec3 v, Vec3 x, Vec3 y, Vec3 z) {
    return vadd(
	vadd(
	    vmul(x, v.x),
	    vmul(y, v.y)),
	vmul(z, v.z)
	);
}

void vprint(Vec3 v) {
    printf("(%.3f, %.3f, %.3f)\n", v.x, v.y, v.z);
}

Color vec3_to_color(Vec3 v) {
    Color rval;
    rval.r = .5f + .5f * v.x;
    rval.g = .5f + .5f * v.y;
    rval.b = .5f + .5f * v.z;
    return rval;
}

