#include <stdio.h>
#include "image_buffer.h"
#include "window.h"
#include "geometry.h"
#include "shading.h"
#include "camera.h"
#include "multithreading.h"
#include <stdlib.h>
#include <sys/sysinfo.h>

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
    Color white = {1.0f, 1.0f, 1.0f};
    Color cream = {50.0f, 45.0f, 40.0f};
    Color red = {1.0f, .0f, .0f};
    Color green = {0.0f, 1.0f, .0f};
    Color yellow = {1.0f, 1.0f, .0f};
    Color emissive_blue = {25.0f, 25.0f, 50.0f};

    BSDF lambert = {
	.f = lambert_bsdf,
	.sampler = cosine_bsdf_sampler,
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
    Camera cam = {
	.kind = PERSPECTIVE,
	.camera = {
	    .perspective = create_perspective_camera(
		(Vec3) {-8.0f, 0.0f, 3.0f},
		(Vec3) {0.0f, 0.0f, 3.0f},
		ratio,
		70.0f)
	}
    };

    Sampler sampler;
    sampler.seed = 0;
    
    Scene sc = {
	.object_count = 8,
	.objects = (Object[]) {
	    {
		.kind = GEO_SPHERE,
		.geometry = {
		    .sphere = {
			.center = {0.0f, 0.0f, 6.0f},
			.radius = .5f,
		    }
		},
		.material = &emissive_mat,
	    },
	    {
		.kind = GEO_PLANE,
		.geometry = {
		    .plane = {
			.normal = normalized((Vec3){.0f, .0f, 1.0f}),
			.distance = .0f,
		    }
		},
		.material = &white_mat,
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
		.kind = GEO_PLANE,
		.geometry = {
		    .plane = {
			.normal = {0.0f, 1.0f, 0.0f},
			.distance = -3.0f
		    }
		},
		.material = &green_mat
	    },
	    // back
	    {
		.kind = GEO_PLANE,
		.geometry = {
		    .plane = {
			.normal = {-1.0f, 0.0f, 0.0f},
			.distance = -3.0f
		    }
		},
		.material = &white_mat
	    },
	    // left
	    {
		.kind = GEO_PLANE,
		.geometry = {
		    .plane = {
			.normal = {0.0f, -1.0f, 0.0f},
			.distance = -3.0f
		    }
		},
		.material = &red_mat
	    },
	    // ceiling
	    {
		.kind = GEO_PLANE,
		.geometry = {
		    .plane = {
			.normal = {0.0f, 0.0f, -1.0f},
			.distance = -6.0f
		    }
		},
		.material = &white_mat
	    }
	},
	.light_count = 1,
	.lights = &l,

	.sampler = &sampler
    };
    
    clear_buffer(buffer);
    bool quit = false;
    int samples = 0;
    int t0 = SDL_GetTicks();
    int n_cores = get_nprocs();
    while (!quit) {
	int t = SDL_GetTicks();

	handle_events(&quit);
		
	//sample_scene(&sc, cam, buffer);
	sample_scene_master(&sc, cam, buffer, n_cores); 
	
	render_buffer(window, buffer);
	SDL_UpdateWindowSurface(window);
	
	int now = SDL_GetTicks();
	char total_time[256];
	format_time((now - t0) / 1000, total_time);
	printf("%6d samples | last %4dms | total %s\r",
	       ++samples,
	       now - t,
	       total_time);
	fflush(stdout);
    }
}

typedef struct Options {
    int width;
    int height;
    char* output_file;
} Options;

Options default_options() {
    return (Options) {
	.width = 200,
	.height = 200,
	.output_file = NULL
    };
}

bool parse_args(int argc, char** argv, Options* options) {
    int i = 1;
    while (i < argc) {
	if (!strcmp(argv[i], "-w") || !strcmp(argv[i], "-h")) {
	    if (i + 1 >= argc) {
		fprintf(stderr, "Invalid syntax\n");
		return false;
	    }
	    int sz = atoi(argv[i + 1]);
	    if (sz <= 0) {
		fprintf(stderr, "Invalid size '%s'\n", argv[i + 1]);
		return false;
	    }
	    if (argv[i][1] == 'w')
		options->width = sz;
	    else
		options->height = sz;
	    i += 2;
	} else if (!strcmp(argv[i], "-o")) {
	    if (i + 1 >= argc) {
		fprintf(stderr, "Invalid syntax\n");
		return false;
	    }
	    options->output_file = argv[i + 1];
	    i += 2;
	} else {
	    fprintf(stderr, "Invalid argument '%s'\n", argv[i]);
	    return false;
	}
    }
}


int main(int argc, char** argv) {
    Options options = default_options();
    if (!parse_args(argc, argv, &options)) {
	return -1;
    }
    
    ImageBuffer* buf = create_buffer(options.width, options.height, GAMMA);

    SDL_Window* window = create_window("", options.width, options.height);

    render_stuff(buf, window);

    if (options.output_file)
	write_image_file(buf, options.output_file);
}

