#pragma once

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

Color mix(Color a, Color b, float t);
Color cadd(Color a, Color b);
Color cscale(Color a, float f);
Color cmul(Color a, Color b);
Color gray(float d);

