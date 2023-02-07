// renderer.h
#pragma once

#include "display.h"
#include "vec.h"
#include "mesh.h"

typedef struct Triangle {
  Vec3 a, b, c;
  uint32_t color;
} Triangle;

typedef enum CullMode {
  CULL_MODE_NONE = 0,
  CULL_MODE_BACKFACE = 1,
} CullMode;

typedef enum DrawMode {
  DRAW_MODE_TRIANGLE_FILL = 0b00000001u,
  DRAW_MODE_TRIANGLE_WIRE = 0b00000010u,
  DRAW_MODE_POINTS =        0b00000100u,
} DrawMode;

typedef struct Renderer {
  Display* display;
  Triangle* renderable_triangles;
  Vec3 camera_position;
  uint32_t clear_color;
  DrawMode draw_mode;
  CullMode cull_mode;
  int display_half_width;
  int display_half_height;
} Renderer;

Renderer* init_renderer(Display* display);
void destroy_renderer(Renderer* renderer);

void renderer_cull_mode(Renderer* renderer, CullMode cull_mode);
void renderer_draw_mode(Renderer* renderer, DrawMode draw_mode);
void renderer_camera_position(Renderer* renderer, Vec3 position);
void renderer_clear_color(Renderer* renderer, uint32_t color);

void renderer_begin_frame(Renderer* renderer);
void renderer_end_frame(Renderer* renderer);

void renderer_begin_triangles(Renderer *renderer, TriangleFace* faces, size_t num_faces,
  Vec3* vertices, size_t num_vertices, Vec3* normals, size_t num_normals);

void renderer_end_triangles(Renderer *renderer);
