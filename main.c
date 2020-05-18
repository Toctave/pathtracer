#include <stdio.h>
#include "image_buffer.h"
#include "window.h"
#include "geometry.h"
#include <stdlib.h>

#define WIDTH 640
#define HEIGHT 480

float rnd() {
    return (float) rand() / RAND_MAX;
}

void handle_events(int* quit) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
	if (event.type == SDL_QUIT ||
	    (event.type == SDL_KEYDOWN) && (event.key.keysym.sym == SDLK_ESCAPE))
	    *quit = 1;
    }
}

void render_scene(ImageBuffer* buffer) {
    static int x = 0;
    static int y = 0;

    Sphere s;
    s.center.x = 0.0f;
    s.center.y = 0.0f;
    s.center.z = 0.0f;

    s.radius = .5f;
    
    clear_buffer(buffer);
    for (int y = 0; y < buffer->height; y++) {
	 for (int x = 0; x < buffer->width; x++) {
	     Ray r = {};
	     r.o.x = -1.0f + 2.0f * ((float) x + .5f) / buffer->width;
	     r.o.y = -1.0f + 2.0f * ((float) y + .5f) / buffer->height;
	     r.o.z = 1.0f;

	     r.d.x = 0.0f;
	     r.d.y = 0.0f;
	     r.d.z = -1.0f;

	     Intersect it;
	     intersect_sphere(s, r, &it);

	     if (it.hit) {
		 buffer->data[y * buffer->width + x] =
		     vec3_to_color(it.normal);
	     }
	 }
    }
}

int main(int argc, char** argv) {
    ImageBuffer* buf = create_buffer(WIDTH, HEIGHT);

    SDL_Window* window = create_window("coucou", WIDTH, HEIGHT);

    int quit = 0;
    while (!quit) {
	handle_events(&quit);

	render_scene(buf);

	render_buffer(window, buf);
	SDL_UpdateWindowSurface(window);
    }
}
