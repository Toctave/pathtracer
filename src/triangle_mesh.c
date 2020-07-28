#include "triangle_mesh.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

bool read_obj_file(TriangleMesh* mesh, const char* filename) {
    FILE* file = fopen(filename, "r");

    char line_buffer[256];

    int vertex_count = 0,
	normal_count = 0;
    mesh->triangle_count = 0;
    
    while (fgets(line_buffer, 256, file)) {
	if (!strncmp(line_buffer, "v ", 2)) {
	    vertex_count++;
	} else 	if (!strncmp(line_buffer, "f ", 2)) {
	    mesh->triangle_count++;
	} else if (!strncmp(line_buffer, "vn ", 3)) {
	    normal_count++;
	}
    }

    Vec3 vertices[vertex_count];
    Vec3 normals[normal_count];
    mesh->triangles = malloc(sizeof(Triangle) * mesh->triangle_count);

    fseek(file, 0, SEEK_SET);

    int vi = 0, vni = 0, fi = 0, line_number = 1;
    int vis[3], vnis[3], trash;
    while (fgets(line_buffer, 256, file)) {
	if (!strncmp(line_buffer, "v ", 2)) {
	    if (sscanf(line_buffer, "v %f %f %f",
		       &vertices[vi].x, &vertices[vi].y, &vertices[vi].z) != 3) {
		fprintf(stderr, "Could not parse vertex line %d in file '%s'\n", line_number, filename);
		return false;
	    }
	    vi++;
	} else 	if (!strncmp(line_buffer, "f ", 2)) {
	    if (sscanf(line_buffer, "f %d/%d/%d %d/%d/%d %d/%d/%d",
		       &vis[0], &trash, &vnis[0],
		       &vis[1], &trash, &vnis[1],
		       &vis[2], &trash, &vnis[2]) != 9 &&
		sscanf(line_buffer, "f %d//%d %d//%d %d//%d",
		       &vis[0], &vnis[0],
		       &vis[1], &vnis[1],
		       &vis[2], &vnis[2]) != 6
		) {
		fprintf(stderr, "Could not parse face line %d in file '%s'\n", line_number, filename);
		return false;
	    } else {
		for (int i = 0; i < 3; i++) {
		    mesh->triangles[fi].vertices[i] = vertices[vis[i]];
		    mesh->triangles[fi].vertex_normals[i] = normals[vnis[i]];
		}
	    }
	    fi++;
	} else if (!strncmp(line_buffer, "vn ", 3)) {
	    if (sscanf(line_buffer, "vn %f %f %f",
		       &normals[vni].x, &normals[vni].y, &normals[vni].z) != 3) {
		fprintf(stderr, "Could not parse vertex normal line %d in file '%s'\n", line_number, filename);
		return false;
	    }
	    vni++;
	}
	line_number++;
    }
    mesh->bvh_root = NULL;
    return true;
}

int compare_by_x(const void* lhs, const void* rhs) {
    Vec3* l = (Vec3*) lhs;
    Vec3* r = (Vec3*) rhs;

    if (l->x < r->x) {
	return -1;
    } else if (l->x > r->x) {
	return 1;
    } else {
	return 0;
    }
}

int compare_by_y(const void* lhs, const void* rhs) {
    Vec3* l = (Vec3*) lhs;
    Vec3* r = (Vec3*) rhs;

    if (l->y < r->y) {
	return -1;
    } else if (l->y > r->y) {
	return 1;
    } else {
	return 0;
    }
}

int compare_by_z(const void* lhs, const void* rhs) {
    Vec3* l = (Vec3*) lhs;
    Vec3* r = (Vec3*) rhs;

    if (l->z < r->z) {
	return -1;
    } else if (l->z > r->z) {
	return 1;
    } else {
	return 0;
    }
}

void build_bvh(TriangleMesh* mesh) {
    Vec3 centroids[mesh->triangle_count];
    for (int i = 0; i < mesh->triangle_count; i++) {
	centroids[i] = vadd(
	    vadd(
		mesh->triangles[i].vertices[0],
		mesh->triangles[i].vertices[1]
		),
	    mesh->triangles[i].vertices[2]);
	centroids[i] = vdiv(centroids[i], 3.0f);
    }
    Vec3* centroids_by_x = malloc(sizeof(Vec3) * mesh->triangle_count);
    Vec3* centroids_by_y = malloc(sizeof(Vec3) * mesh->triangle_count);
    Vec3* centroids_by_z = malloc(sizeof(Vec3) * mesh->triangle_count);

    qsort(centroids_by_x, mesh->triangle_count, sizeof(Vec3), &compare_by_x);
    qsort(centroids_by_y, mesh->triangle_count, sizeof(Vec3), &compare_by_y);
    qsort(centroids_by_z, mesh->triangle_count, sizeof(Vec3), &compare_by_z);

    /* @TODO : build BVH nodes */

    free(centroids_by_x);
    free(centroids_by_y);
    free(centroids_by_z);
}
