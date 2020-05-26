#pragma once
#include <stdbool.h>

typedef struct Color {
    float r;
    float g;
    float b;
} Color;

typedef struct PixelData {
    Color color;
    int samples;
} PixelData;

typedef struct ImageBuffer {
    int width;
    int height;
    PixelData* data;
} ImageBuffer;


ImageBuffer* create_buffer(int width, int height);
void clear_buffer(ImageBuffer* buffer);
void add_pixel_sample(ImageBuffer* buffer, int x, int y, Color c);
bool write_image_file(ImageBuffer* buffer, char* filepath, float gamma);
void dump_pixel_data(ImageBuffer* buffer, unsigned char* pixels, float gamma);
void rgb_pixel_value(ImageBuffer* buffer, int i,
		     unsigned char* r,
		     unsigned char* g,
		     unsigned char* b,
		     float inv_gamma);

Color mix(Color a, Color b, float t);
Color gray(float d);

static inline Color cadd(Color a, Color b) {
    Color rval = {
	a.r + b.r,
	a.g + b.g,
	a.b + b.b
    };
    return rval;
}

static inline Color cmul(Color a, Color b) {
    return (Color) {
	a.r * b.r,
	a.g * b.g,
	a.b * b.b
    };
}

static inline Color cscale(Color a, float f) {
    return (Color) {a.r * f, a.g * f, a.b * f};
}

static inline void clamp(Color* color) {
    if (color->r < 0.0f) color->r = 0.0f;
    if (color->r > 1.0f) color->r = 1.0f;
    if (color->g < 0.0f) color->g = 0.0f;
    if (color->g > 1.0f) color->g = 1.0f;
    if (color->b < 0.0f) color->b = 0.0f;
    if (color->b > 1.0f) color->b = 1.0f;
}
