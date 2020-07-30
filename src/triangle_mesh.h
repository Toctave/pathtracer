#pragma once

#include "geometry.h"
#include <stdbool.h>

struct TriangleMesh;

typedef struct BVHNode {
    Vec3 vmin;
    Vec3 vmax;
    bool* indices;
    struct TriangleMesh* mesh;

    struct BVHNode* left;
    struct BVHNode* right;
} BVHNode;

typedef struct TriangleMesh {
    Triangle* triangles;
    unsigned int triangle_count;
    BVHNode* bvh_root;
} TriangleMesh;

typedef struct Centroid {
    Vec3 p;
    int idx;
} Centroid;

bool read_obj_file(TriangleMesh* mesh, const char* filename);
void build_bvh(TriangleMesh* mesh);
