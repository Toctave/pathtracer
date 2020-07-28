#pragma once

#include "geometry.h"
#include <stdbool.h>

typedef struct LLTriangle {
    unsigned int index;
    struct LLTriangle* next;
} LLTriangle;

typedef struct BVHNode {
    Vec3 vmin;
    Vec3 vmax;
    Triangle** triangles;
    unsigned int triangle_count;
    struct BVHNode* left;
    struct BVHNode* right;
} BVHNode;

typedef struct TriangleMesh {
    Triangle* triangles;
    unsigned int triangle_count;
    BVHNode* bvh_root;
} TriangleMesh;

bool read_obj_file(TriangleMesh* mesh, const char* filename);
void build_bvh(TriangleMesh* mesh);
