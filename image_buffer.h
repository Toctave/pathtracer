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
    float gamma;
} ImageBuffer;


ImageBuffer* create_buffer(int width, int height, float gamma);
void clear_buffer(ImageBuffer* buffer);
void add_pixel_sample(ImageBuffer* buffer, int x, int y, Color c);
bool write_image_file(ImageBuffer* buffer, char* filepath);
void raw_pixel_data(ImageBuffer* buffer, unsigned char* pixels);

Color mix(Color a, Color b, float t);
Color cadd(Color a, Color b);
Color cscale(Color a, float f);
Color cmul(Color a, Color b);
Color gray(float d);
void clamp(Color* color);

