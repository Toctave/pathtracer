# pathtracer

This is a basic pathtracer I wrote in C99 as an exercise. It aims at keeping things simple while maintaining good performance (both in terms of algorithms and memory/cache management). 

## Features
- Basic Shapes : spheres, bounded and unbounded planes, triangle meshes
- BVH acceleration for meshes, with surface area heuristic and reasonably fast build times (about 2s for 1M tris on a single Ryzen 5 3600 core)
- Basic Materials : pure lambertian, pure reflection
- Global illumination
- Cosine-weighted and uniform sampling
- Multi-threaded rendering

### To do :
- ~Faster BVH build times & smarter heuristics~
- Smarter sampling schemes (Multiple Importance Sampling, bidirectionnal pathtracing, Metropolis Light Transport)
- More interesting BSDFs (glass, microfacet models, BSDF mixes)
- Scene-wide BVHs
- Better data- and task- parallelism (SIMD, maybe OpenCL?)
- User friendliness (scene file specification, interactive interface)
- *General system stability improvements to enhance the user's experience* 

## Building

The only dependencies are SDL2, for simple cross-platform window management, and stb_image (included in the source) for writing image files.

    git clone https://github.com/Toctave/pathtracer.git
    cd pathtracer
    make
    
## Usage

    bin/pathtracer [-w <image width>] [-h <image height>] [-s <max samples>] [-t <max render time (s)>] [-o <output file>]
