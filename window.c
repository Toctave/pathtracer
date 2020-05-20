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

void clamp(Color* color) {
    if (color->r < 0.0f) color->r = 0.0f;
    if (color->r > 1.0f) color->r = 1.0f;
    if (color->g < 0.0f) color->g = 0.0f;
    if (color->g > 1.0f) color->g = 1.0f;
    if (color->b < 0.0f) color->b = 0.0f;
    if (color->b > 1.0f) color->b = 1.0f;
}


void render_buffer(SDL_Window* window, ImageBuffer* buffer) {
    unsigned char* pixels = malloc(buffer->width *
				   buffer->height *
				   3);
    for (int i = 0; i < buffer->width * buffer->height; i++) {
	Color c = buffer->data[i];
	clamp(&c);
    	pixels[3 * i] = (unsigned char) (sqrtf(c.r) * 255.0f);
    	pixels[3 * i + 1] = (unsigned char) (sqrtf(c.g) * 255.0f);
	pixels[3 * i + 2] = (unsigned char) (sqrtf(c.b) * 255.0f);
    }

    Uint32 rmask, gmask, bmask, amask;
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
    rmask = 0xff0000;
    gmask = 0x00ff00;
    bmask = 0x0000ff;
#else
    rmask = 0x0000ff;
    gmask = 0x00ff00;
    bmask = 0xff0000;
#endif

    SDL_Surface* surface =
	SDL_CreateRGBSurfaceFrom(pixels,
				 buffer->width,
				 buffer->height,
				 24,
				 buffer->width * 3,
				 rmask,
				 gmask,
				 bmask,
				 0);

    int err = SDL_BlitSurface(surface, NULL,
		    SDL_GetWindowSurface(window), NULL);
    if (err < 0)
	log_sdl_error("Failed to blit image");

    free(pixels);
}
