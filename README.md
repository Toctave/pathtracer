# pathtracer

This is a basic pathtracer I wrote in pure C as an exercise.

## Features
- Basic Shapes : spheres, infinite planes, finite planes
- Triangle meshes with BVH acceleration
- Basic Materials : pure lambertian, pure reflection
- Global illumination
- Cosine-weighted and uniform sampling
- Multi-threading

### To do :
- Complex BSDFs
- Faster BVH build times & smarter heuristics
- Scene-wide BVHs

## Building

The only dependency is SDL2, for simple cross-platform window management, and stb_image (included in the source) for writing image files.

    git clone https://github.com/Toctave/pathtracer.git
    cd pathtracer
    make
    
## Usage

    bin/pathtracer [-w <image width>] [-h <image height>] [-s <max samples>] [-t <max render time>] [-o <output file>]
