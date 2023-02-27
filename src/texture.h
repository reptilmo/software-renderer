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

bool texture_load_checker_board(Texture* texture);
bool texture_load_tga(Texture* texture, const char* tga_file_path);
