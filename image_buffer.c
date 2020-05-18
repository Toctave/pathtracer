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

