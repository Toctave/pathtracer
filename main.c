#include <stdio.h>
#include "image_buffer.h"
#include "window.h"
#include "geometry.h"
#include "shading.h"
#include "camera.h"
#include <stdlib.h>

#define WIDTH 640
#define HEIGHT 480

void handle_events(int* quit) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
	if (event.type == SDL_QUIT ||
	    (event.type == SDL_KEYDOWN) && (event.key.keysym.sym == SDLK_ESCAPE))
	    *quit = 1;
    }
}

void render_stuff(ImageBuffer* buffer) {
    static int x = 0;
    static int y = 0;

    Object objects[2];

    Sphere s = {
	.center = {.0f, .0f, .5f},
	.radius = .5f,
    };
    Plane p = {
	.normal = normalized((Vec3){.0f, .0f, 1.0f}),
	.distance = .0f,
    };

    BSDF lambert = {
	.f = lambert_bsdf,
	.sampler = uniform_bsdf_sampler,
    };
    Color white = {1.0f, 1.0f, 1.0f};
    Material white_mat = {
	.bsdf = &lambert,
	.params = &white
    };

    objects[0] = (Object) {
	.kind = GEO_SPHERE,
	.geometry = { .sphere = s },
	.material = &white_mat,
    };

    objects[1] = (Object) {
	.kind = GEO_PLANE,
	.geometry = { .plane = p },
	.material = &white_mat,
    };
    
    Light l = {
	.position = {.0f, 1.0f, 2.0f},
	.color = {.3f, 1.0f, .5f},
	.intensity = 3.0f,
    };
    
    float ratio = (float) WIDTH / HEIGHT;
    OrthographicCamera cam = create_ortho_camera(
	(Vec3) {-1.0f, 0.0f, 2.0f},
	(Vec3) {0.0f, 0.0f, 0.0f},
	ratio,
	2.0f);
    
    Scene sc = {
	.object_count = 2,
	.objects = objects,
	.light_count = 1,
	.lights = &l,
    };

    clear_buffer(buffer);
    render_scene(&sc, cam, buffer);
}

int main(int argc, char** argv) {
    ImageBuffer* buf = create_buffer(WIDTH, HEIGHT);

    SDL_Window* window = create_window("", WIDTH, HEIGHT);

    int t = SDL_GetTicks();
    render_stuff(buf);
    int dt = SDL_GetTicks() - t;
    printf("Rendered in %.3fs\n", (float) dt / 1000);

    int quit = 0;
    while (!quit) {
	handle_events(&quit);
	
	render_buffer(window, buf);
	SDL_UpdateWindowSurface(window);
    }
}
