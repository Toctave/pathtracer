#include <stdio.h>
#include "image_buffer.h"
#include "window.h"
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

int main(int argc, char** argv) {
    ImageBuffer* buf = create_buffer(WIDTH, HEIGHT);

    SDL_Window* window = create_window("coucou", WIDTH, HEIGHT);

    int quit = 0;
    while (!quit) {
	handle_events(&quit);
	for (int i = 0; i < buf->width * buf->height; i++) {
	    buf->data[i].r = rnd();
	    buf->data[i].g = rnd();
	    buf->data[i].b = rnd();
	    buf->data[i].a = 1.0f;
	}
	render_buffer(window, buf);
	SDL_UpdateWindowSurface(window);
    }
}
