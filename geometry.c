#include "geometry.h"
#include <math.h>
#include <stdio.h>

float norm2(Vec3 v) {
    return v.x * v.x + v.y * v.y + v.z * v.z;
}

float norm(Vec3 v) {
    return sqrtf(norm2(v));
}


float dot(Vec3 a, Vec3 b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}


Vec3 vadd(Vec3 a, Vec3 b) {
    Vec3 rval = {
	a.x + b.x,
	a.y + b.y,
	a.z + b.z
    };
    return rval;
}

Vec3 vsub(Vec3 a, Vec3 b) {
    Vec3 rval = {
	a.x - b.x,
	a.y - b.y,
	a.z - b.z
    };
    return rval;
}

Vec3 vmul(Vec3 v, float t) {
    Vec3 rval = {
	v.x * t,
	v.y * t,
	v.z * t
    };
    return rval;
}

Vec3 vdiv(Vec3 v, float t) {
    Vec3 rval = {
	v.x / t,
	v.y / t,
	v.z / t
    };
    return rval;
}

Vec3 along_ray(Ray r, float t) {
    return vadd(r.o, vmul(r.d, t));
}

void vprint(Vec3 v) {
    printf("(%.3f, %.3f, %.3f)\n", v.x, v.y, v.z);
}

Color vec3_to_color(Vec3 v) {
    Color rval;
    rval.r = .5f + .5f * v.x;
    rval.g = .5f + .5f * v.y;
    rval.b = .5f + .5f * v.z;
    rval.a = 1.0f;
    return rval;
}

void intersect_sphere(Sphere s, Ray r, Intersect* intersect) {
    Vec3 center_orig = vsub(r.o, s.center);
    float a = norm2(r.d);
    float b = 2.0f * dot(center_orig, r.d);
    float c = norm2(center_orig) - s.radius * s.radius;

//    vprint(center_orig);
//    vprint(r.d);

    float delta = b * b - 4 * a * c;
//    printf("%.3f %.3f %.3f, %.3f\n", a, b, c, delta);

    if (delta < 0) {
	intersect->hit = false;
    } else {
	float sqd = sqrtf(delta);
	float t = .5f * (-b - sqd) / a;
	if (t > EPSILON) {
	    intersect->hit = true;
	    intersect->point = along_ray(r, t);
	    intersect->normal = vdiv(
		vsub(intersect->point, s.center),
		s.radius);
	    return;
	}

	t += sqd / a;
	if (t > EPSILON) {
	    intersect->hit = true;
	    intersect->point = along_ray(r, t);
	    intersect->normal = vdiv(
		vsub(s.center, intersect->point),
		s.radius);
	    return;
	}
	intersect->hit = false;
    }
}
 
