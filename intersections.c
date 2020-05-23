#include "intersections.h"
#include <math.h>

bool intersect_sphere(Sphere s, Ray r, Intersect* intersect) {
    Vec3 center_orig = vsub(r.o, s.center);
    float a = norm2(r.d);
    float b = 2.0f * dot(center_orig, r.d);
    float c = norm2(center_orig) - s.radius * s.radius;

    float delta = b * b - 4 * a * c;

    intersect->outgoing = vneg(r.d);

    if (delta > 0) {
	float sqd = sqrtf(delta);
	float t = .5f * (-b - sqd) / a;
	if (t > EPSILON && t < intersect->t) {
	    intersect->hit = true;
	    intersect->t = t;
	    intersect->point = along_ray(r, t);
	    intersect->normal = vdiv(
		vsub(intersect->point, s.center),
		s.radius);
	    return true;
	} else if ((t += sqd / a) > EPSILON && t < intersect->t) {
	    intersect->hit = true;
	    intersect->t = t;
	    intersect->point = along_ray(r, t);
	    intersect->normal = vdiv(
		vsub(s.center, intersect->point),
		s.radius);
	    return true;
	}
    }
    return false;
}
 
bool intersect_plane(Plane p, Ray r, Intersect* intersect) {
    // (o + t * d) . n = D
    float d = dot(p.normal, r.d);
    intersect->outgoing = vneg(r.d);
    if (d < -EPSILON || d > EPSILON) {
	float t = (p.distance - dot(r.o, p.normal)) / d;
	if (t > EPSILON && t < intersect->t) {
	    intersect->hit = true;
	    intersect->t = t;
	    intersect->point = along_ray(r, t);
	    intersect->normal = d > 0 ? vneg(p.normal) : p.normal;
	    return true;
	}
    }
    return false;
}

bool intersects_plane(Plane p, Ray r) {
    float d = dot(p.normal, r.d);
    if (d < -EPSILON || d > EPSILON) {
	float t = (p.distance - dot(r.o, p.normal)) / d;
	if (t > EPSILON) {
	    return true;
	}
    }
    return false;
}

