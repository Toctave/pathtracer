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
    struct Color* data;
} ImageBuffer;


ImageBuffer* create_buffer(int width, int height);
void mix(struct Color a, struct Color b);
