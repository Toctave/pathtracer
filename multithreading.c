#include "multithreading.h"
#include <stdio.h>

void sample_scene_master(Scene* scene, Camera camera, ImageBuffer* buffer, Sampler* samplers, int num_threads) {
    pthread_t threads[num_threads];
    SceneSampleInfo infos[num_threads];
    
    int band_height = buffer->height / num_threads + 1;
    pthread_mutex_t buffer_mutex = PTHREAD_MUTEX_INITIALIZER;
    for (int i = 0 ; i < num_threads; i++) {
	int ymax_candidate = (i + 1) * band_height;
	infos[i] = (SceneSampleInfo) {
	    .xmin = 0,
	    .xmax = buffer->width,
	    .ymin = i * band_height,
	    .ymax = ymax_candidate < buffer->height ? ymax_candidate : buffer->height,

	    .buffer = buffer,
	    .buffer_mutex = buffer_mutex,

	    .scene = scene,
	    .camera = camera,
	    .samplers = samplers
	};
	pthread_create(&threads[i],
		       NULL,
		       sample_scene_slave,
		       (void *) &infos[i]);
    }

    for (int i = 0 ; i < num_threads + 1; i++) {
	pthread_join(threads[i], NULL);
    }
}

void* sample_scene_slave(void* data) {
    SceneSampleInfo* ss = (SceneSampleInfo*) data;

    for (int y = ss->ymin; y < ss->ymax; y++) {
	for (int x = ss->xmin; x < ss->xmax; x++) {
	    Sampler* sampler = &ss->samplers[y * ss->buffer->width + x];
	    float dx, dy;
	    sample_unit_square(sampler, &dx, &dy, NULL);
	    float sx = (x + dx) / ss->buffer->width;
	    float sy = 1.0f - (y + dy) / ss->buffer->height;
	    Ray r = camera_ray(ss->camera, sx, sy);

	    //pthread_mutex_lock(&ss->buffer_mutex);
	    add_pixel_sample(ss->buffer, x, y,
			     trace_ray(ss->scene, r, 0, sampler).outgoing_radiance);
	    //pthread_mutex_unlock(&ss->buffer_mutex);
	}
    }

    return NULL;
}


