#pragma once

#include "geometry.h"
#include <stdbool.h>
#include <stdlib.h>

struct TriangleMesh;

typedef struct LLNode {
    void* data;
    struct LLNode* next;
} LLNode;

typedef struct BVHNode {
    Vec3 vmin;
    Vec3 vmax;
    struct TriangleMesh* mesh;

    LLNode* indices_head;
    struct BVHNode* left;
    struct BVHNode* right;
} BVHNode;

typedef struct TriangleMesh {
    Triangle* triangles;
    size_t triangle_count;
    BVHNode* bvh_root;
} TriangleMesh;

typedef struct Centroid {
    Vec3 p;
    size_t idx;
} Centroid;

bool read_obj_file(TriangleMesh* mesh, const char* filename);
void build_bvh(TriangleMesh* mesh);
