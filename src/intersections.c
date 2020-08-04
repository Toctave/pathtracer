#include "intersections.h"
#include <math.h>
#include <stdio.h>

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
    if (d < -EPSILON || d > EPSILON) {
        float t = (p.distance - dot(r.o, p.normal)) / d;
        if (t > EPSILON && t < intersect->t) {
            intersect->hit = true;
            intersect->t = t;
            intersect->point = along_ray(r, t);
            intersect->normal = d > 0 ? vneg(p.normal) : p.normal;
            intersect->outgoing = vneg(r.d);
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

bool intersect_partial_plane(PartialPlane p, Ray r, Intersect* intersect) {
    float d = dot(p.basis[2], r.d);
    if (d < -EPSILON || d > EPSILON) {
        float t = dot(vsub(p.origin, r.o), p.basis[2]) / d;
        if (t > EPSILON && t < intersect->t) {
            Vec3 hit_point = along_ray(r, t);
            Vec3 origin_to_hit = vsub(hit_point, p.origin);
            float u = dot(origin_to_hit, p.basis[0]);
            float v = dot(origin_to_hit, p.basis[1]);

            if (u > p.min[0] && v > p.min[1] &&
                u < p.max[0] && v < p.max[1]) {
                intersect->hit = true;
                intersect->t = t;
                intersect->point = along_ray(r, t);
                intersect->normal = d > 0 ? vneg(p.basis[2]) : p.basis[2];
                intersect->outgoing = vneg(r.d);
                return true;
            }
        }
    }
    return false;
}

bool intersects_partial_plane(PartialPlane p, Ray r) {
    float d = dot(p.basis[2], r.d);
    if (d < -EPSILON || d > EPSILON) {
        float t = dot(vsub(p.origin, r.o), p.basis[2]) / d;
        if (t > EPSILON) {
            Vec3 hit_point = along_ray(r, t);
            Vec3 origin_to_hit = vsub(hit_point, p.origin);
            float u = dot(origin_to_hit, p.basis[0]);
            float v = dot(origin_to_hit, p.basis[1]);

            if (u > p.min[0] && v > p.min[1] &&
                u < p.max[0] && v < p.max[1]) {
                return true;
            }
        }
    }
    return false;
}

bool intersect_triangle(Triangle tri, Ray r, Intersect* intersect) {
    Vec3 e1 = vsub(tri.vertices[1], tri.vertices[0]),
        e2 = vsub(tri.vertices[2], tri.vertices[0]);

    Vec3 pvec = cross(r.d, e2);

    float det = dot(e1, pvec);
    if (det > -EPSILON && det < EPSILON) {
        return false;
    }
    float inv_det = 1.0f / det;

    Vec3 tvec = vsub(r.o, tri.vertices[0]);

    float u = dot(tvec, pvec) * inv_det;
    if (u < 0.0f || u > 1.0f) {
        return false;
    }

    Vec3 qvec = cross(tvec, e1);
    float v = dot(r.d, qvec) * inv_det;
    if (v < 0.0f || u + v > 1.0f) {
        return false;
    }

    float t = dot(e2, qvec) * inv_det;
    if (t > EPSILON && t < intersect->t) {
        intersect->t = t;
        intersect->hit = true;
        intersect->point = along_ray(r, intersect->t);
        intersect->normal = normalized(
            vadd(
                vadd(
                    vmul(tri.vertex_normals[0], 1 - u - v),
                    vmul(tri.vertex_normals[1], u)
                    ),
                vmul(tri.vertex_normals[2], v)
                )
            );
	    
        intersect->outgoing = vneg(r.d);
        return true;
    } else {
        return false;
    }
}

bool intersects_triangle(Triangle t, Ray r) {
    Vec3 e1 = vsub(t.vertices[1], t.vertices[0]),
        e2 = vsub(t.vertices[2], t.vertices[0]);

    Vec3 pvec = cross(r.d, e2);

    float det = dot(e1, pvec);
    if (det > -EPSILON && det < EPSILON) {
        return false;
    }
    float inv_det = 1.0f / det;

    Vec3 tvec = vsub(r.o, t.vertices[0]);

    float u = dot(tvec, pvec) * inv_det;
    if (u < 0.0f || u > 1.0f) {
        return false;
    }

    Vec3 qvec = cross(tvec, e1);
    float v = dot(r.d, qvec) * inv_det;
    if (v < 0.0f || v > 1.0f) {
        return false;
    }
    
    return true;
}

bool intersects_box(Vec3 vmin, Vec3 vmax, Ray r) {
    Vec3 bounds[2] = {vmin, vmax};
    float tmin, tmax, tymin, tymax, tzmin, tzmax;
    bool sign[3];
    sign[0] = r.d.x < 0.0f;
    sign[1] = r.d.y < 0.0f;
    sign[2] = r.d.z < 0.0f;
    Vec3 invdir = {
        1.0f / r.d.x,
        1.0f / r.d.y,
        1.0f / r.d.z
    };
 
    tmin = (bounds[sign[0]].x - r.o.x) * invdir.x; 
    tmax = (bounds[!sign[0]].x - r.o.x) * invdir.x; 
    tymin = (bounds[sign[1]].y - r.o.y) * invdir.y; 
    tymax = (bounds[!sign[1]].y - r.o.y) * invdir.y; 
 
    if ((tmin > tymax) || (tymin > tmax)) 
        return false; 
    if (tymin > tmin) 
        tmin = tymin; 
    if (tymax < tmax) 
        tmax = tymax; 
 
    tzmin = (bounds[sign[2]].z - r.o.z) * invdir.z; 
    tzmax = (bounds[!sign[2]].z - r.o.z) * invdir.z; 
 
    if ((tmin > tzmax) || (tzmin > tmax)) 
        return false; 
 
    return true; 
}

bool intersect_bvh(BVHNode* node, Ray r, Intersect* intersect) {
    if (!intersects_box(node->vmin, node->vmax, r)) {
        return false;
    }
    
    if (!node->left) {
        bool did_intersect = false;
        for (int i = 0; i < node->triangle_count; i++) {
            if (intersect_triangle(node->triangles[i], r, intersect))
                did_intersect = true;
        }
        return did_intersect;
    }

    bool did_intersect = false;
    if (intersect_bvh(node->left, r, intersect))
        did_intersect = true;
    if (intersect_bvh(node->right, r, intersect))
        did_intersect = true;

    return did_intersect;
}

bool intersects_bvh(BVHNode* node, Ray r) {
    if (!intersects_box(node->vmin, node->vmax, r))
        return false;
    
    if (!node->left) {
        for (int i = 0; i < node->triangle_count; i++) {
            if (intersects_triangle(node->triangles[i], r))
                return true;
        }
        return false;
    }

    if (intersects_bvh(node->left, r))
        return true;
    if (intersects_bvh(node->right, r))
        return true;

    return false;
}

#define USE_BVH 1

bool intersect_triangle_mesh(TriangleMesh mesh, Ray r, Intersect* intersect) {
#if USE_BVH
    return intersect_bvh(mesh.bvh_root, r, intersect);
#else
    bool did_intersect = false;
    for (int i = 0; i < mesh.triangle_count; i++) {
        if (intersect_triangle(mesh.triangles[i], r, intersect)) {
            did_intersect = true;
        }
    }
    return did_intersect;
#endif
}

bool intersects_triangle_mesh(TriangleMesh mesh, Ray r) {
#if USE_BVH
    return intersects_bvh(mesh.bvh_root, r);
#else
    bool did_intersect = false;
    for (int i = 0; i < mesh.triangle_count; i++) {
        if (intersects_triangle(mesh.triangles[i], r)) {
            did_intersect = true;
        }
    }
    return did_intersect;
#endif
}

