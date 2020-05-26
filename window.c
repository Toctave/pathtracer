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

void render_buffer(SDL_Window* window, ImageBuffer* buffer, float gamma) {
    SDL_Surface* window_surface = SDL_GetWindowSurface(window);
    Uint32* pixels = (Uint32 *) window_surface->pixels;
    SDL_LockSurface(window_surface);
    float inv_gamma = 1.0f / gamma;
    for (int i = 0; i < buffer->width * buffer->height; i++) {
	unsigned char r, g, b;
	rgb_pixel_value(buffer, i, &r, &g, &b, inv_gamma);
	pixels[i] = SDL_MapRGBA(window_surface->format, r, g, b, 255);
    }
    SDL_UnlockSurface(window_surface);
    SDL_UpdateWindowSurface(window);
}
