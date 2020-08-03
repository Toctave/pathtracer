#include "triangle_mesh.h"
#include "debug.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

bool read_obj_file(TriangleMesh* mesh, const char* filename) {
    FILE* file = fopen(filename, "r");

    if (!file) {
	fprintf(stderr, "Could not open file '%s'\n", filename);
	return false;
    }

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

    Vec3* vertices = malloc(sizeof(Vec3) * vertex_count);
    Vec3* normals = malloc(sizeof(Vec3) * normal_count);
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
		    mesh->triangles[fi].vertices[i] = vertices[vis[i] - 1];
		    mesh->triangles[fi].vertex_normals[i] = normals[vnis[i] - 1];
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

    free(vertices);
    free(normals);
    
    printf("Building BVH for mesh '%s'...\n", filename);
    build_bvh(mesh);
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

void build_bvh_node(BVHNode* node, TriangleMesh* mesh, bool* indices,
		    Centroid* centroids_by_x, Centroid* centroids_by_y, Centroid* centroids_by_z,
		    unsigned int depth, Triangle** new_triangles) {
    node->vmin.x = INFINITY;
    node->vmin.y = INFINITY;
    node->vmin.z = INFINITY;

    node->vmax.x = -INFINITY;
    node->vmax.y = -INFINITY;
    node->vmax.z = -INFINITY;

    Centroid* centroids_by_split_axis = NULL;
    unsigned int split_axis = depth % 3;
    switch (split_axis) {
    case 0:
	centroids_by_split_axis = centroids_by_x;
	break;
    case 1:
	centroids_by_split_axis = centroids_by_y;
	break;
    case 2:
	centroids_by_split_axis = centroids_by_z;
	break;
    }

    int triangle_count = 0;

    for (size_t i = 0; i < mesh->triangle_count; i++) {
	int idx = centroids_by_split_axis[i].idx;
	if (indices[idx]) {
	    for (int j = 0; j < 3; j++) {
		if (mesh->triangles[idx].vertices[j].x < node->vmin.x) {
		    node->vmin.x = mesh->triangles[idx].vertices[j].x;
		}
		if (mesh->triangles[idx].vertices[j].x > node->vmax.x) {
		    node->vmax.x = mesh->triangles[idx].vertices[j].x;
		}

		if (mesh->triangles[idx].vertices[j].y < node->vmin.y) {
		    node->vmin.y = mesh->triangles[idx].vertices[j].y;
		}
		if (mesh->triangles[idx].vertices[j].y > node->vmax.y) {
		    node->vmax.y = mesh->triangles[idx].vertices[j].y;
		}

		if (mesh->triangles[idx].vertices[j].z < node->vmin.z) {
		    node->vmin.z = mesh->triangles[idx].vertices[j].z;
		}
		if (mesh->triangles[idx].vertices[j].z > node->vmax.z) {
		    node->vmax.z = mesh->triangles[idx].vertices[j].z;
		}
	    }
	    triangle_count++;
	}
    }

    if (triangle_count > 10 && depth < 20) {
	bool* l_indices = malloc(sizeof(bool) * mesh->triangle_count);
	bool* r_indices = malloc(sizeof(bool) * mesh->triangle_count);

	int triangles_seen = 0;
	for (size_t i = 0; i < mesh->triangle_count; i++) {
	    size_t idx = centroids_by_split_axis[i].idx;
	    l_indices[idx] = false;
	    r_indices[idx] = false;
	    if (indices[idx]) {
		if (triangles_seen < triangle_count / 2) {
		    l_indices[idx] = true;
		} else {
		    r_indices[idx] = true;
		}
		triangles_seen++;
	    }
	}

	node->left = malloc(sizeof(BVHNode));
	build_bvh_node(node->left, mesh, l_indices,
		       centroids_by_x, centroids_by_y, centroids_by_z,
		       depth + 1, new_triangles);
	node->right = malloc(sizeof(BVHNode));
	build_bvh_node(node->right, mesh, r_indices,
			   centroids_by_x, centroids_by_y, centroids_by_z,
		       depth + 1, new_triangles);
	free(r_indices);
	free(l_indices);
    } else {
	node->left = NULL;
	node->right = NULL;

	node->triangles = *new_triangles;
	node->triangle_count = 0;
	for (size_t i = 0; i < mesh->triangle_count; i++) {
	    if (indices[i]) {
		node->triangles[node->triangle_count++] = mesh->triangles[i];
	    }
	}
	*new_triangles += node->triangle_count;
	/* printf("%zu triangles processed\r", triangles_processed); */
    }
}

void build_bvh(TriangleMesh* mesh) {
    Centroid* centroids = malloc(sizeof(Centroid) * mesh->triangle_count);
    bool* indices = malloc(sizeof(bool) * mesh->triangle_count);
    for (size_t i = 0; i < mesh->triangle_count; i++) {
	indices[i] = true;
	centroids[i].idx = i;
	centroids[i].p = vadd(
	    vadd(
		mesh->triangles[i].vertices[0],
		mesh->triangles[i].vertices[1]
		),
	    mesh->triangles[i].vertices[2]);
	centroids[i].p = vdiv(centroids[i].p, 3.0f);
    }
    Centroid* centroids_by_x = malloc(sizeof(Centroid) * mesh->triangle_count);
    Centroid* centroids_by_y = malloc(sizeof(Centroid) * mesh->triangle_count);
    Centroid* centroids_by_z = malloc(sizeof(Centroid) * mesh->triangle_count);

    memcpy(centroids_by_x, centroids, sizeof(Centroid) * mesh->triangle_count);
    memcpy(centroids_by_y, centroids, sizeof(Centroid) * mesh->triangle_count);
    memcpy(centroids_by_z, centroids, sizeof(Centroid) * mesh->triangle_count);

    qsort(centroids_by_x, mesh->triangle_count, sizeof(Centroid), &compare_by_x);
    qsort(centroids_by_y, mesh->triangle_count, sizeof(Centroid), &compare_by_y);
    qsort(centroids_by_z, mesh->triangle_count, sizeof(Centroid), &compare_by_z);

    Triangle* new_triangles = malloc(sizeof(Triangle) * mesh->triangle_count);

    mesh->bvh_root = malloc(sizeof(BVHNode));
    build_bvh_node(mesh->bvh_root, mesh, indices,
    		   centroids_by_x, centroids_by_y, centroids_by_z,
    		   0, &new_triangles);

    free(mesh->triangles);
    mesh->triangles = new_triangles;

    free(centroids_by_x);
    free(centroids_by_y);
    free(centroids_by_z);
    
    free(centroids);
    free(indices);
}
