#include "image_buffer.h"
#include <stdlib.h>
#include <math.h>
#include "globals.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

ImageBuffer* create_buffer(int width, int height) {
    ImageBuffer* rval =
        malloc(sizeof(ImageBuffer) +
               width * height * sizeof(PixelData));
    rval->data = (PixelData*) (rval + 1);
    rval->width = width;
    rval->height = height;
    return rval;
}

void clear_buffer(ImageBuffer* buffer) {
    for (int i = 0; i < buffer-> width * buffer->height; i++) {
        buffer->data[i].color.r = 0.0f;
        buffer->data[i].color.g = 0.0f;
        buffer->data[i].color.b = 0.0f;
        buffer->data[i].weight_sum = 0.0f;
    }
}

float sinc(float x) {
    if (fabs(x) < 1e-5) return 1.0f;
    return sinf(PI * x) / (PI * x);
}

float windowed_sinc(float x, float tau, float radius) {
    x = fabs(x);
    if (x > radius) return 0.0f;
    return sinc(x) * sinc(x / tau);
}

float falloff(float dx, float dy, float extent) {
    static const float tau = 3.0f;
    return windowed_sinc(dx, tau, extent) * windowed_sinc(dy, tau, extent);
//    return fabs(dx) < .5f && fabs(dy) < .5f ? 1.0f : 0.0f;
//    float d = sqrtf(dx * dx + dy * dy);
//    return d < 1.0f ? (1.0f - d) * 3.0f / PI : 0.0f;
//    static const float alpha = 2.0f;
    //return expf(- (dx * dx + dy * dy) * alpha) - expf(-alpha * extent * extent);
//    return sinf(d) / d;
}

void add_pixel_sample(ImageBuffer* buffer, int x, int y, float dx, float dy, Color c) {
    /* static const int extent = 3.0f; */
    /* for (int ny = -extent; ny <= extent; ny++) { */
	/* for (int nx = -extent; nx <= extent; nx++) { */
    /* if (x + nx >= 0 && x + nx < buffer->width && */
    /* y + ny >= 0 && y + ny < buffer->height) { */
    /* float ddx = dx - .5f - nx; */
    /* float ddy = dy - .5f - ny; */
    /* float weight = falloff(ddx, ddy, (float) extent); */
	    
    /* int i = (y + ny) * buffer->width + (x + nx); */
    int i = y * buffer->width + x;
    buffer->data[i].weight_sum += 1.0f;
    buffer->data[i].color = cadd(buffer->data[i].color, c);
    /* cscale(c, weight)); */
    /* } */
	/* } */
    /* } */
}

void rgb_pixel_value(ImageBuffer* buffer, int i,
                     unsigned char* r,
                     unsigned char* g,
                     unsigned char* b,
                     float inv_gamma) {
    Color c = cscale(buffer->data[i].color,
                     1.0f / buffer->data[i].weight_sum);
    clamp(&c);
    *r = 
        (unsigned char) (powf(c.r, inv_gamma) * 255.0f);
    *g =
        (unsigned char) (powf(c.g, inv_gamma) * 255.0f);
    *b = 
        (unsigned char) (powf(c.b, inv_gamma) * 255.0f);

}

void dump_pixel_data(ImageBuffer* buffer, unsigned char* pixels, float gamma) {
    const int channels = 3;
    float inv_gamma = 1.0f / gamma;
    for (int i = 0; i < buffer->width * buffer->height; i++) {
        if (buffer->data[i].weight_sum == 0.0f) {
            continue;
        }
        rgb_pixel_value(buffer, i,
                        &pixels[channels * i],
                        &pixels[channels * i + 1],
                        &pixels[channels * i + 2],
                        inv_gamma);
    }
}

bool write_image_file(ImageBuffer* buffer, char* filepath, float gamma) {
    const int channels = 3;
    unsigned char* pixels = malloc(buffer->width *
                                   buffer->height *
                                   channels);
    dump_pixel_data(buffer, pixels, gamma);
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

