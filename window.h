#pragma once

#include <SDL2/SDL.h>
#include "image_buffer.h"

void log_sdl_error(char* message);
SDL_Window* create_window(char* title, int width, int height);
void render_buffer(SDL_Window* window, ImageBuffer* buffer);

