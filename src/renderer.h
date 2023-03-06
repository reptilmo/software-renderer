// renderer.h
#pragma once

#include "display.h"
#include "mat.h"
#include "mesh.h"
#include "texture.h"
#include "triangle.h"
#include "frustum.h"

typedef enum CullMode {
  CULL_MODE_NONE = 0,
  CULL_MODE_BACKFACE = 1,
} CullMode;

typedef enum DrawMode {
  DRAW_MODE_TRIANGLE_FILL = 0b00000001u,
  DRAW_MODE_TRIANGLE_WIRE = 0b00000010u,
  DRAW_MODE_POINTS = 0b00000100u,
  DRAW_MODE_TEXTURE = 0b00001000u,
} DrawMode;

typedef enum LightMode {
  LIGHT_MODE_NONE = 0,
  LIGHT_MODE_FLAT = 1,
  LIGHT_MODE_GOURAUD = 2,
} LightMode;

typedef struct Renderer {
  Display* display;
  Texture* current_texture;
  Polygon* renderable_triangles;
  Vec3 light_direction;
  Vec3 camera_position;
  uint32_t clear_color;
  DrawMode draw_mode;
  CullMode cull_mode;
  LightMode light_mode;
  Frustum view_frustum;
  Mat4 projection_matrix;
  float view_half_width;
  float view_half_height;
} Renderer;

Renderer* init_renderer(Display* display);
void destroy_renderer(Renderer* renderer);

void renderer_cull_mode(Renderer* renderer, CullMode cull_mode);
void renderer_draw_mode(Renderer* renderer, DrawMode draw_mode);
void renderer_light_mode(Renderer* renderer, LightMode light_mode);
void renderer_camera_position(Renderer* renderer, const Vec3* position);
void renderer_light_direction(Renderer* renderer, const Vec3* light_direction);
void renderer_clear_color(Renderer* renderer, uint32_t color);
void renderer_current_texture(Renderer* renderer, Texture* texture);

void renderer_begin_frame(Renderer* renderer);
void renderer_end_frame(Renderer* renderer);

void renderer_begin_triangles(Renderer* renderer, TriangleFace* faces, size_t num_faces,
                              Vec3* vertices, size_t num_vertices, Vec3* normals, size_t num_normals, Vec2* uvs, size_t num_uvs);

void renderer_end_triangles(Renderer* renderer);
