#include <stdio.h>
#include "image_buffer.h"
#include "window.h"
#include "geometry.h"
#include "shading.h"
#include "camera.h"
#include <stdlib.h>

#define WIDTH 640
#define HEIGHT 480
#define GAMMA 2.0f

void handle_events(bool* quit) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
	if (event.type == SDL_QUIT ||
	    (event.type == SDL_KEYDOWN) && (event.key.keysym.sym == SDLK_ESCAPE))
	    *quit = 1;
    }
}

void format_time(int seconds, char* buffer) {
    int minutes = seconds / 60;
    seconds = seconds % 60;
    int hours = minutes / 60;
    minutes = minutes % 60;

    sprintf(buffer, "%02dh %02dm %02ds", hours, minutes, seconds);
}

void render_stuff(ImageBuffer* buffer, SDL_Window* window) {
    static int x = 0;
    static int y = 0;

    Object objects[3];

    Sphere lamp_sphere = {
	.center = {-2.0f, 4.0f, 2.0f},
	.radius = 1.0f,
    };
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
	.emitted = NULL
    };
    Color white = {1.0f, 1.0f, 1.0f};
    Color red = {1.0f, .0f, .0f};

    Color emissive_blue = {25.0f, 25.0f, 50.0f};
    Material white_mat = {
	.bsdf = &lambert,
	.params = &white
    };
    Material red_mat = {
	.bsdf = &lambert,
	.params = &red
    };

    BSDF emissive = {
	.f = NULL,
	.sampler = NULL,
	.emitted = isotropic_emission
    };
    Material emissive_mat = {
	.bsdf = &emissive,
	.params = &emissive_blue
    };

    objects[0] = (Object) {
	.kind = GEO_SPHERE,
	.geometry = { .sphere = lamp_sphere },
	.material = &emissive_mat,
    };

    objects[1] = (Object) {
	.kind = GEO_PLANE,
	.geometry = { .plane = p },
	.material = &white_mat,
    };

    objects[2] = (Object) {
	.kind = GEO_SPHERE,
	.geometry = { .sphere = s },
	.material = &red_mat
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
	.object_count = 3,
	.objects = objects,
	.light_count = 1,
	.lights = &l,
    };
    
    clear_buffer(buffer);
    bool quit = false;
    int samples = 0;
    int t0 = SDL_GetTicks();
    while (!quit) {
	handle_events(&quit);
	
	int t = SDL_GetTicks();
	sample_scene(&sc, cam, buffer);
	int now = SDL_GetTicks();
	char total_time[256];
	format_time((now - t0) / 1000, total_time);
	printf("%6d samples | last %4dms | total %s\n",
	       ++samples,
	       now - t,
	       total_time);
	
	render_buffer(window, buffer);
	SDL_UpdateWindowSurface(window);
    }
}

int main(int argc, char** argv) {
    ImageBuffer* buf = create_buffer(WIDTH, HEIGHT, GAMMA);

    SDL_Window* window = create_window("", WIDTH, HEIGHT);

    render_stuff(buf, window);
}
