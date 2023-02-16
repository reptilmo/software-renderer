// renderer.h
#pragma once

#include "display.h"
#include "texture.h"
#include "mesh.h"
#include "vec.h"
#include "mat.h"

typedef struct Triangle {
  Vec3 a, b, c;
  Vec2 a_uv, b_uv, c_uv;
  uint32_t color;
} Triangle;

typedef enum CullMode {
  CULL_MODE_NONE = 0,
  CULL_MODE_BACKFACE = 1,
} CullMode;

typedef enum DrawMode {
  DRAW_MODE_TRIANGLE_FILL = 0b00000001u,
  DRAW_MODE_TRIANGLE_WIRE = 0b00000010u,
  DRAW_MODE_POINTS        = 0b00000100u,
  DRAW_MODE_TEXTURE       = 0b00001000u,
} DrawMode;

typedef struct Renderer {
  Display* display;
  Triangle* renderable_triangles;
  Texture* current_texture;
  Vec3 light_direction;
  Vec3 camera_position;
  uint32_t clear_color;
  DrawMode draw_mode;
  CullMode cull_mode;
  Mat4 projection_matrix;
  float view_half_width;
  float view_half_height;
} Renderer;

Renderer* init_renderer(Display* display);
void destroy_renderer(Renderer* renderer);

void renderer_cull_mode(Renderer* renderer, CullMode cull_mode);
void renderer_draw_mode(Renderer* renderer, DrawMode draw_mode);
void renderer_camera_position(Renderer* renderer, Vec3 position);
void renderer_light_direction(Renderer* renderer, Vec3 light_direction);
void renderer_clear_color(Renderer* renderer, uint32_t color);
void renderer_current_texture(Renderer* renderer, Texture* texture);

void renderer_begin_frame(Renderer* renderer);
void renderer_end_frame(Renderer* renderer);

void renderer_begin_triangles(Renderer* renderer, TriangleFace* faces, size_t num_faces,
  Vec3* vertices, size_t num_vertices, Vec3* normals, size_t num_normals, Vec2* uvs, size_t num_uvs);

void renderer_end_triangles(Renderer* renderer);
