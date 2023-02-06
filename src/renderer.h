// renderer.h
#pragma once

#include "display.h"
#include "vec.h"
#include "mesh.h"

typedef struct Triangle {
  Vec3 a, b, c;
  uint32_t color;
} Triangle;

typedef struct Renderer {
  Display* display;
  Triangle* renderable_triangles;
  Vec3 camera_position;
  uint32_t clear_color;
  int display_half_width;
  int display_half_height;
  bool cull_backface;
} Renderer;

Renderer* init_renderer(Display* display);
void destroy_renderer(Renderer* renderer);

void renderer_backface_culling(Renderer* renderer, bool cull_backface);
void renderer_camera_position(Renderer* renderer, Vec3 postion);
void renderer_clear_color(Renderer* renderer, uint32_t color);

void renderer_begin_frame(Renderer* renderer);
void renderer_end_frame(Renderer* renderer);

void renderer_begin_triangles(Renderer *renderer, TriangleFace* faces, size_t num_faces,
  Vec3* vertices, size_t num_vertices, Vec3* normals, size_t num_normals);

void renderer_end_triangles(Renderer *renderer);
