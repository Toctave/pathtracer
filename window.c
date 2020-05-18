#include "window.h"

void log_sdl_error(char* message) {
    fprintf(stderr, "%s\n  SDL Error message : %s\n", message, SDL_GetError());
}

SDL_Window* create_window(char* title, int width, int height) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
	log_sdl_error("Failed to initialize SDL!");
	return NULL;
    }

    SDL_Window* window = SDL_CreateWindow(title,
			      SDL_WINDOWPOS_UNDEFINED,
			      SDL_WINDOWPOS_UNDEFINED,
			      width,
			      height,
			      SDL_WINDOW_SHOWN);
    if (!window) {
	log_sdl_error("Failed to initialize window!");
	return NULL;
    }
    return window;
}

void render_buffer(SDL_Window* window, ImageBuffer* buffer) {
    unsigned char* pixels = malloc(buffer->width *
				   buffer->height *
				   4);
    for (int i = 0; i < buffer->width * buffer->height; i++) {
	pixels[4 * i]     = (unsigned char) (buffer->data[i].r * 255.0f);
    	pixels[4 * i + 1] = (unsigned char) (buffer->data[i].g * 255.0f);
	pixels[4 * i + 2] = (unsigned char) (buffer->data[i].b * 255.0f);
	pixels[4 * i + 3] = (unsigned char) (buffer->data[i].a * 255.0f);
    }

    Uint32 rmask, gmask, bmask, amask;
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
    rmask = 0xff000000;
    gmask = 0x00ff0000;
    bmask = 0x0000ff00;
    amask = 0x000000ff;
#else
    rmask = 0x000000ff;
    gmask = 0x0000ff00;
    bmask = 0x00ff0000;
    amask = 0xff000000;
#endif

    SDL_Surface* surface =
	SDL_CreateRGBSurfaceFrom(pixels,
				 buffer->width,
				 buffer->height,
				 32,
				 buffer->width * 3,
				 rmask,
				 gmask,
				 bmask,
				 amask);

    int err = SDL_BlitSurface(surface, NULL,
		    SDL_GetWindowSurface(window), NULL);
    if (err < 0)
	log_sdl_error("Failed to blit image");

    free(pixels);
}
