#pragma once

typedef struct Color {
    float r;
    float g;
    float b;
    float a;
} Color;

typedef struct ImageBuffer {
    int width;
    int height;
    Color* data;
} ImageBuffer;


ImageBuffer* create_buffer(int width, int height);
void clear_buffer(ImageBuffer* buffer);
Color mix(Color a, Color b, float t);
Color cadd(Color a, Color b);
Color gray(float d);

