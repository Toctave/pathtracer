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
    rval->inv_gamma = 1.0f / gamma;
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

void rgb_pixel_value(ImageBuffer* buffer, int i,
		     unsigned char* r,
		     unsigned char* g,
		     unsigned char* b) {
    Color c = cscale(buffer->data[i].color,
		     1.0f / buffer->data[i].samples);
    clamp(&c);
    *r = 
	(unsigned char) (powf(c.r, buffer->inv_gamma) * 255.0f);
    *g =
	(unsigned char) (powf(c.g, buffer->inv_gamma) * 255.0f);
    *b = 
	(unsigned char) (powf(c.b, buffer->inv_gamma) * 255.0f);

}

void dump_pixel_data(ImageBuffer* buffer, unsigned char* pixels) {
    const int channels = 3;
    for (int i = 0; i < buffer->width * buffer->height; i++) {
	if (!buffer->data[i].samples) {
	    continue;
	}
	rgb_pixel_value(buffer, i,
			&pixels[channels * i],
			&pixels[channels * i + 1],
			&pixels[channels * i + 2]);
    }
}

bool write_image_file(ImageBuffer* buffer, char* filepath) {
    const int channels = 3;
    unsigned char* pixels = malloc(buffer->width *
				   buffer->height *
				   channels);
    dump_pixel_data(buffer, pixels);
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

