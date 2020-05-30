#include <stdio.h>
#include "image_buffer.h"
#include "window.h"
#include "geometry.h"
#include "shading.h"
#include "camera.h"
#include "multithreading.h"
#include <stdlib.h>
#include <sys/sysinfo.h>

#define GAMMA 2.2f

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

void render_stuff(ImageBuffer* buffer,
		  SDL_Window* window,
		  int max_samples,
		  int max_seconds,
		  bool uniform_sampling) {
    Color white = {1.0f, 1.0f, 1.0f};
    Color cream = cscale((Color){1.0f, .9f, .8f}, 20.0f);
    Color red = {1.0f, .0f, .0f};
    Color green = {0.0f, 1.0f, .0f};
    Color yellow = {1.0f, 1.0f, .0f};
    Color emissive_blue = {25.0f, 25.0f, 50.0f};

    BSDF lambert = {
	.f = lambert_bsdf,
	.sampler = uniform_sampling ? uniform_bsdf_sampler : cosine_weighted_bsdf_sampler,
	.emitted = NULL
    };
    Material white_mat = {
	.bsdf = &lambert,
	.params = &white
    };
    Material red_mat = {
	.bsdf = &lambert,
	.params = &red
    };
    Material green_mat = {
	.bsdf = &lambert,
	.params = &green
    };
    Material yellow_mat = {
	.bsdf = &lambert,
	.params = &yellow
    };

    BSDF emissive = {
	.f = NULL,
	.sampler = NULL,
	.emitted = isotropic_emission
    };
    Material emissive_mat = {
	.bsdf = &emissive,
	.params = &cream
    };

    BSDF mirror = {
	.f = perfect_reflection_bsdf,
	.sampler = perfect_reflection_sampler,
	.emitted = NULL
    };
    Material mirror_mat = {
	.bsdf = &mirror,
	.params = NULL
    };
    
    Light l = {
	.position = {.0f, 1.0f, 2.0f},
	.color = {.3f, 1.0f, .5f},
	.intensity = 3.0f,
    };
    
    float ratio = (float) buffer->width / buffer->height;
    float fov = 90.0f;
    Camera cam = {
	.kind = PERSPECTIVE,
	.camera = {
	    .perspective = create_perspective_camera(
		(Vec3) {-7.5f, 0.0f, 3.0f},
		(Vec3) {0.0f, 0.0f, 3.0f},
		ratio,
		fov)
	}
    };

    Scene sc = {
	.objects = (Object[]) {
	    {
		.kind = GEO_PARTIAL_PLANE,
		.geometry = {
		    .partial_plane = create_partial_plane(
			(Vec3) {0.0f, 0.0f, 5.95f},
			(Vec3) {1.0f, 0.0f, 0.0f},
			(Vec3) {0.0f, 1.0f, 0.0f},
			-2, -2,
			2, 2
		    )
		},
		.material = &emissive_mat,
	    },
	    {
		.kind = GEO_SPHERE,
		.geometry = {
		    .sphere = {
			.center = {1.3f, -.5f, 1.5f},
			.radius = 1.5f,
		    }
		},
		.material = &yellow_mat
	    },
	    {
		.kind = GEO_SPHERE,
		.geometry = {
		    .sphere = {
			.center = {.5f, 1.0f, 4.0f},
			.radius = 1.0f,
		    }
		},
		.material = &mirror_mat
	    },
	    // WALLS
	    // right
	    {
		.kind = GEO_PARTIAL_PLANE,
		.geometry = {
		    .partial_plane = create_partial_plane(
			(Vec3) {0.0f, -3.0f, 3.0f},
			(Vec3) {1.0f, 0.0f, 0.0f},
			(Vec3) {0.0f, 0.0f, 1.0f},
			-3, -3,
			3, 3
		    )
		},
		.material = &green_mat,
	    },
	    // back
	    {
		.kind = GEO_PARTIAL_PLANE,
		.geometry = {
		    .partial_plane = create_partial_plane(
			(Vec3) {3.0f, 0.0f, 3.0f},
			(Vec3) {0.0f, 1.0f, 0.0f},
			(Vec3) {0.0f, 0.0f, 1.0f},
			-3, -3,
			3, 3
		    )
		},
		.material = &white_mat
	    },
	    // left
	    {
		.kind = GEO_PARTIAL_PLANE,
		.geometry = {
		    .partial_plane = create_partial_plane(
			(Vec3) {0.0f, 3.0f, 3.0f},
			(Vec3) {1.0f, 0.0f, 0.0f},
			(Vec3) {0.0f, 0.0f, 1.0f},
			-3, -3,
			3, 3
		    )
		},
		.material = &red_mat
	    },
	    // floor
	    {
		.kind = GEO_PARTIAL_PLANE,
		.geometry = {
		    .partial_plane = create_partial_plane(
			(Vec3) {0.0f, 0.0f, 0.0f},
			(Vec3) {1.0f, 0.0f, 0.0f},
			(Vec3) {0.0f, 1.0f, 0.0f},
			-3, -3,
			3, 3
		    )
		},
		
		.material = &white_mat
	    },
	    // ceiling
	    {
		.kind = GEO_PARTIAL_PLANE,
		.geometry = {
		    .partial_plane = create_partial_plane(
			(Vec3) {0.0f, 0.0f, 6.0f},
			(Vec3) {1.0f, 0.0f, 0.0f},
			(Vec3) {0.0f, 1.0f, 0.0f},
			-3, -3,
			3, 3
		    )
		},
		.material = &white_mat
	    }
	},
	.light_count = 1,
	.lights = &l,
    };
    sc.object_count = sizeof(sc.objects);
    
    
    clear_buffer(buffer);
    bool quit = false;
    long int samples = 0;
    long int t0 = SDL_GetTicks();
    int n_cores = get_nprocs();
    printf("Detected %d cores, rendering with %d threads.\n", n_cores, n_cores);
    Sampler* samplers = malloc(sizeof(Sampler) * buffer->width * buffer->height);
    create_samplers(samplers, buffer->width * buffer->height);
    while (!quit) {
	int t = SDL_GetTicks();

	handle_events(&quit);
		
	//sample_scene(&sc, cam, buffer);
	sample_scene_master(&sc, cam, buffer, samplers, n_cores); 
	
	render_buffer(window, buffer, GAMMA);
	SDL_UpdateWindowSurface(window);

	++samples;
	long int now = SDL_GetTicks();
	char total_time[256];
	long int total_seconds = (now - t0) / 1000;
	format_time(total_seconds, total_time);
	printf("%6ld samples | last %4ldms | total %s\r",
	       ++samples,
	       now - t,
	       total_time);
	fflush(stdout);

	if ((max_samples > 0 && samples >= max_samples) ||
	    (max_seconds > 0 && total_seconds >= max_seconds)) {
	    quit = true;
	}
    }
    free(samplers);
}

typedef struct Options {
    long width;
    long height;
    long max_samples;
    long max_seconds;
    char* output_file;
    bool uniform_sampling;
} Options;

Options default_options() {
    return (Options) {
	.width = 200,
	.height = 200,
	.output_file = NULL,
	.max_samples = -1,
	.max_seconds = -1,
	.uniform_sampling = false
    };
}

bool parse_args(int argc, char** argv, Options* options) {
    int i = 1;
    while (i < argc) {
	if (!strcmp(argv[i], "-w") ||
	    !strcmp(argv[i], "-h") ||
	    !strcmp(argv[i], "-s") ||
	    !strcmp(argv[i], "-t")) {
	    if (i + 1 >= argc) {
		fprintf(stderr, "Invalid syntax\n");
		return false;
	    }
	    long int value = atoi(argv[i + 1]);
	    if (value <= 0) {
		fprintf(stderr, "Invalid value '%s' for parameter '%s'\n", argv[i + 1], argv[i]);
		return false;
	    }
	    if (!strcmp(argv[i], "-w"))
		options->width = value;
	    else if (!strcmp(argv[i], "-h"))
		options->height = value;
	    else if (!strcmp(argv[i], "-s"))
		options->max_samples = value;
	    else if (!strcmp(argv[i], "-t"))
		options->max_seconds = value;
	    
	    i += 2;
	} else if (!strcmp(argv[i], "-o")) {
	    if (i + 1 >= argc) {
		fprintf(stderr, "Invalid syntax\n");
		return false;
	    }
	    options->output_file = argv[i + 1];
	    i += 2;
	} else if (!strcmp(argv[i], "-u")) {
	    options->uniform_sampling = true;
	    i++;
	} else {
	    fprintf(stderr, "Invalid argument '%s'\n", argv[i]);
	    return false;
	}
    }
    return true;
}

void test_samplers() {
    Sampler s;
    create_samplers(&s, 1);
    for (int i = 0; i < 2 * STRATIFIED_RESOLUTION * STRATIFIED_RESOLUTION; i++) {
	float x, y;
	sample_unit_square(&s, &x, &y, NULL);
	printf("%f %f\n", x, y);
    }
}


int main(int argc, char** argv) {
    Options options = default_options();
    if (!parse_args(argc, argv, &options)) {
	return -1;
    }
    ImageBuffer* buf = create_buffer(options.width, options.height);

    SDL_Window* window = create_window(options.output_file ? options.output_file : "Pathtracer", options.width, options.height);

    render_buffer(window, buf, GAMMA);
    render_stuff(buf, window, options.max_samples, options.max_seconds, options.uniform_sampling);
    printf("\n");

    if (options.output_file)
	write_image_file(buf, options.output_file, GAMMA);
    else
	write_image_file(buf, "latest.png", GAMMA);
}

