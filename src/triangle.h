// triangle.h
#pragma once

#include "vec.h"
#include "display.h"
#include "texture.h"

typedef struct Triangle {
  Vec4 points[3];
  Vec2 uvs[3];
  uint32_t color;
  float light_intensity;
} Triangle;

void draw_triangle_shaded(Display* display, const Triangle* triangle);
void draw_triangle_textured(Display* display, const Triangle* triangle, const Texture* texture);

