// triangle.h
#pragma once

#include "vec.h"
#include "display.h"
#include "texture.h"

enum {
  POLYGON_MAX_VERTICES = 9,
};

typedef struct Polygon {
  Vec4 vertices[POLYGON_MAX_VERTICES];
  Vec2 uvs[POLYGON_MAX_VERTICES];
  int vertex_count;
  float light_intensity;
  uint32_t color;
} Polygon;

typedef struct Triangle {
  Vec4 points[3];
  Vec2 uvs[3];
  float light_intensity;
  uint32_t color;
} Triangle;

void draw_triangle_shaded(Display* display, const Triangle* triangle);
void draw_triangle_textured(Display* display, const Triangle* triangle, const Texture* texture);

