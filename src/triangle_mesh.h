#pragma once

#include "geometry.h"
#include <stdbool.h>
#include <stdlib.h>

struct TriangleMesh;

typedef struct LLNodeIdx {
    unsigned int idx;
    struct LLNodeIdx* next;
} LLNodeIdx;

typedef struct BVHNode {
    Vec3 vmin;
    Vec3 vmax;
    
    bool isLeaf;
    union {
        struct {
            Triangle* array;
            int count;
        } triangles;
        struct {
            struct BVHNode* left;
            struct BVHNode* right;
        } children;
    } data;
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
