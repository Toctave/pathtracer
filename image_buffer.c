#include "image_buffer.h"
#include <stdlib.h>

ImageBuffer* create_buffer(int width, int height) {
    ImageBuffer* rval =
	malloc(sizeof(ImageBuffer) +
	       width * height * sizeof(struct Color));
    rval->data = (struct Color*) (rval + 1);
    rval->width = width;
    rval->height = height;
    return rval;
}

void clear_buffer(ImageBuffer* buffer) {
    for (int i = 0; i < buffer-> width * buffer->height; i++) {
	buffer->data[i].r = 0.0f;
	buffer->data[i].g = 0.0f;
	buffer->data[i].b = 0.0f;
	buffer->data[i].a = 1.0f;
    }
}

