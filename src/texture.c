// texture.c
#include "texture.h"

Texture* init_texture() {
  Texture* texture = (Texture*)malloc(sizeof(Texture));
  if (texture != NULL) {
    texture->width = 0;
    texture->height = 0;
  }
  return texture;
}

void destroy_texture(Texture* texture) {
  ASSERT(texture != NULL);
  if (texture != NULL) {
    if (texture->bitmap != NULL) {
      free(texture->bitmap);
      texture->bitmap = NULL;
      texture->width = 0;
      texture->height = 0;
    }
    free(texture);
  }
}

bool texture_load_checker_board(Texture* texture) {
  ASSERT(texture != NULL);
  if (texture == NULL) {
    return false;
  }

  texture->width = 32;
  texture->height = 32;
  texture->bitmap = (uint32_t*)malloc(sizeof(uint32_t) * texture->width * texture->height);
  if (texture->bitmap != NULL) {
    for (size_t i = 0; i < texture->height; i++) {
      if ((i / 4) % 2) {
        for (size_t j = 0; j < texture->width; j += 4) {
          if ((j / 4) % 2) {
            texture->bitmap[texture->width * i + j + 0] = 0xFF555555;
            texture->bitmap[texture->width * i + j + 1] = 0xFF555555;
            texture->bitmap[texture->width * i + j + 2] = 0xFF555555;
            texture->bitmap[texture->width * i + j + 3] = 0xFF555555;
          } else {
            texture->bitmap[texture->width * i + j + 0] = 0xFFFF1111;
            texture->bitmap[texture->width * i + j + 1] = 0xFFFF1111;
            texture->bitmap[texture->width * i + j + 2] = 0xFFFF1111;
            texture->bitmap[texture->width * i + j + 3] = 0xFFFF1111;
          }
        }
      } else {
        for (size_t j = 0; j < texture->width; j += 4) {
          if ((j / 4) % 2) {
            texture->bitmap[texture->width * i + j + 0] = 0xFFFF1111;
            texture->bitmap[texture->width * i + j + 1] = 0xFFFF1111;
            texture->bitmap[texture->width * i + j + 2] = 0xFFFF1111;
            texture->bitmap[texture->width * i + j + 3] = 0xFFFF1111;
          } else {
            texture->bitmap[texture->width * i + j + 0] = 0xFF555555;
            texture->bitmap[texture->width * i + j + 1] = 0xFF555555;
            texture->bitmap[texture->width * i + j + 2] = 0xFF555555;
            texture->bitmap[texture->width * i + j + 3] = 0xFF555555;
          }
        }
      }
    }
    return true;
  }
  return false;
}

uint32_t texture_sample(Texture* texture, Vec2 pixel, Vec2 a, Vec2 b, Vec2 c, Vec2 a_uv, Vec2 b_uv, Vec2 c_uv) {

  const Vec2 AB = vec2_sub(b, a);
  const Vec2 AC = vec2_sub(c, a);
  const Vec2 AP = vec2_sub(pixel, a);
  const Vec2 PB = vec2_sub(b, pixel);
  const Vec2 PC = vec2_sub(c, pixel);

  const float triangle_area_times_2 = AC.x * AB.y - AC.y * AB.x;
  const float weight_a = (PC.x * PB.y - PC.y * PB.x) / triangle_area_times_2;
  const float weight_b = (AC.x * AP.y - AC.y * AP.x) / triangle_area_times_2;
  const float weight_c = 1 - weight_a - weight_b;

  const float s = a_uv.x * weight_a + b_uv.x * weight_b + c_uv.x * weight_c;
  const float t = a_uv.y * weight_a + b_uv.y * weight_b + c_uv.y * weight_c;

  int w = abs((int)(s * texture->width));
  int h = abs((int)(t * texture->height));

  if (w >= texture->width) {
    w = texture->width - 1;
  }

  if (h >= texture->height) {
    h = texture->height - 1;
  }

  ASSERT(w >= 0 && w < texture->width);
  ASSERT(h >= 0 && h < texture->height);

  return texture->bitmap[texture->width * h + w];
}
