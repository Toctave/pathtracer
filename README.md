# pathtracer

This is a basic pathtracer I wrote in pure C as an exercise.

## Features
- Basic Shapes : spheres, infinite planes, finite planes
- Basic Materials : pure lambertian, pure reflection
- Global illumination
- Cosine-weighted and uniform sampling
- Multi-threading

### To do :
- Triangle meshes
- Complex BSDFs

## Building

This uses SDL2 for easy cross-platform window management.

    git clone https://github.com/Toctave/pathtracer.git
    cd pathtracer
    make
    
## Usage

    bin/pathtracer [-w <image width>] [-h <image height>] [-s <total samples>] [-t <total render time>] [-o <output file>]
