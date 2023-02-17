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
uint32_t texture_sample(Texture* texture, Vec2 pixel, Vec4 a, Vec4 b, Vec4 c, Vec2 a_uv, Vec2 b_uv, Vec2 c_uv, float reciprocal_area);

bool texture_load_checker_board(Texture* texture);
bool texture_load_tga(Texture* texture, const char* tga_file_path);
