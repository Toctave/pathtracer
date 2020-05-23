#include "image_buffer.h"
#include <stdlib.h>
#include <math.h>
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

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

void raw_pixel_data(ImageBuffer* buffer, unsigned char* pixels) {
    const int channels = 3;
    float invGamma = 1.0f / buffer->gamma;
    for (int i = 0; i < buffer->width * buffer->height; i++) {
	if (!buffer->data[i].samples) {
	    continue;
	}
	Color c = cscale(buffer->data[i].color,
		       1.0f / buffer->data[i].samples);
	clamp(&c);
    	pixels[channels * i] =
	    (unsigned char) (powf(c.r, invGamma) * 255.0f);
    	pixels[channels * i + 1] = 
	    (unsigned char) (powf(c.g, invGamma) * 255.0f);
	pixels[channels * i + 2] =
	    (unsigned char) (powf(c.b, invGamma) * 255.0f);

	float u = pixels[channels * i] +
	    pixels[channels * i + 1] +
	    pixels[channels * i + 2];
    }
}

bool write_image_file(ImageBuffer* buffer, char* filepath) {
    const int channels = 3;
    unsigned char* pixels = malloc(buffer->width *
				   buffer->height *
				   channels);
    raw_pixel_data(buffer, pixels);
    bool success = !stbi_write_png(
	filepath,
	buffer->width,
	buffer->height,
	channels,
	pixels,
	buffer->width * channels
	);
    free(pixels);
    return success;
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

void clamp(Color* color) {
    if (color->r < 0.0f) color->r = 0.0f;
    if (color->r > 1.0f) color->r = 1.0f;
    if (color->g < 0.0f) color->g = 0.0f;
    if (color->g > 1.0f) color->g = 1.0f;
    if (color->b < 0.0f) color->b = 0.0f;
    if (color->b > 1.0f) color->b = 1.0f;
}

