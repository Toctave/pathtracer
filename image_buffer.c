#include "image_buffer.h"
#include <stdlib.h>
#include <math.h>

ImageBuffer* create_buffer(int width, int height, float gamma) {
    ImageBuffer* rval =
	malloc(sizeof(ImageBuffer) +
	       width * height * sizeof(PixelData));
    rval->data = (PixelData*) (rval + 1);
    rval->width = width;
    rval->height = height;
    rval->gamma = gamma;
    return rval;
}

void clear_buffer(ImageBuffer* buffer) {
    for (int i = 0; i < buffer-> width * buffer->height; i++) {
	buffer->data[i].color.r = 0.0f;
	buffer->data[i].color.g = 0.0f;
	buffer->data[i].color.b = 0.0f;
	buffer->data[i].samples = 0;
    }
}

void add_pixel_sample(ImageBuffer* buffer, int x, int y, Color c) {
    int i = y * buffer->width + x;
    buffer->data[i].samples++;
    buffer->data[i].color.r += powf(c.r, buffer->gamma);
    buffer->data[i].color.g += powf(c.g, buffer->gamma);
    buffer->data[i].color.b += powf(c.b, buffer->gamma);
}


Color gray(float d) {
    Color rval = {d, d, d};
    return rval;
}

Color cadd(Color a, Color b) {
    Color rval = {
	a.r + b.r,
	a.g + b.g,
	a.b + b.b
    };
    return rval;
}

Color cmul(Color a, Color b) {
    return (Color) {
	a.r * b.r,
	a.g * b.g,
	a.b * b.b
    };
}

Color cscale(Color a, float f) {
    return (Color) {a.r * f, a.g * f, a.b * f};
}
