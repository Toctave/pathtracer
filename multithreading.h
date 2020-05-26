#pragma once
#include <pthread.h>
#include "image_buffer.h"
#include "camera.h"
#include "scene.h"

typedef struct SceneSampleInfo {
    // Rectangular zone that will be sampled
    int xmin;
    int xmax;
    int ymin;
    int ymax;

    // Buffer to render to
    ImageBuffer* buffer;
    pthread_mutex_t buffer_mutex;

    // Scene to sample from
    Scene* scene;
    Camera camera;

    Sampler* samplers;
} SceneSampleInfo;

void* sample_scene_slave(void* data);
void sample_scene_master(Scene* scene, Camera camera, ImageBuffer* buffer, Sampler* samplers, int num_threads);
