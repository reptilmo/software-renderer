// texture.h
#pragma once

#include "system.h"
#include "vec.h"

typedef struct Texture {
  uint32_t* bitmap;
  int width;
  int height;
} Texture;

Texture* init_texture(void);
void destroy_texture(Texture* texture);
uint32_t texture_sample(Texture* texture, Vec2 pixel, Vec2 a, Vec2 b, Vec2 c, Vec2 a_uv, Vec2 b_uv, Vec2 c_uv);
bool texture_load_checker_board(Texture* texture);
