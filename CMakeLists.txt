cmake_minimum_required(VERSION 3.10)

project(pathtracer)

add_executable(${PROJECT_NAME})

target_sources(${PROJECT_NAME} PRIVATE
  src/main.c
  src/bsdf.c
  src/camera.c
  src/geometry.c
  src/image_buffer.c
  src/intersections.c
  src/multithreading.c
  src/sampling.c
  src/scene.c
  src/shading.c
  src/triangle_mesh.c
  src/window.c
  src/pcg/pcg_basic.c
  )

target_include_directories(${PROJECT_NAME} PRIVATE
  src/vendor)

target_link_libraries(${PROJECT_NAME} SDL2 m pthread)
