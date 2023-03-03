// texture.c
#include "texture.h"

// https://en.wikipedia.org/wiki/Truevision_TGA
typedef struct TGAHeader {
  uint8_t id_field_length;
  uint8_t color_map_type;
  uint8_t image_type;
  uint8_t color_map_spec[5];
  uint16_t origin_x;
  uint16_t origin_y;
  uint16_t image_width;
  uint16_t image_height;
  uint8_t image_bits_per_pixel;
  uint8_t image_descriptor;
} TGAHeader;

Texture* init_texture() {
  Texture* texture = (Texture*)malloc(sizeof(Texture));
  if (texture != NULL) {
    texture->bitmap = NULL;
    texture->width = 0;
    texture->height = 0;
  }
  return texture;
}

void destroy_texture(Texture* texture) {
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

bool texture_load_tga(Texture* texture, const char* tga_file_path) {
  ASSERT(texture != NULL);
  if (texture == NULL) {
    return false;
  }

  FILE* tga_file = fopen(tga_file_path, "rb");
  if (tga_file == NULL) {
    fprintf(stderr, "Failed to open %s!\n", tga_file_path);
    return false;
  }

  TGAHeader header;
  memset(&header, 0, sizeof(TGAHeader));
  if (fread(&header, sizeof(TGAHeader), 1, tga_file) < 1) {
    fprintf(stderr, "Failed to read %s!\n", tga_file_path);
    fclose(tga_file);
    return false;
  }

  if (header.image_type != 2) {
    fprintf(stderr, "Unsupported image type %s!\n", tga_file_path);
    fclose(tga_file);
    return false;
  }

  const uint8_t bytes_per_pixel = header.image_bits_per_pixel / 8;
  if (bytes_per_pixel != 4) {
    fprintf(stderr, "Unsupported image BPP %u!\n", bytes_per_pixel);
    fclose(tga_file);
    return false;
  }

  texture->width = (int)header.image_width;
  texture->height = (int)header.image_height;

  const size_t image_data_len = texture->width * texture->height * bytes_per_pixel;
  texture->bitmap = (uint32_t*)malloc(sizeof(uint32_t) * image_data_len);
  if (texture->bitmap == NULL) {
    fprintf(stderr, "Failed to allocate image buffer for %s!\n", tga_file_path);
    fclose(tga_file);
    return false;
  }

  if (fread(texture->bitmap, sizeof(uint8_t), image_data_len, tga_file) != image_data_len) {
    fprintf(stderr, "Failed to read data for %s!\n", tga_file_path);
    fclose(tga_file);
    return false;
  }

  return true;
}
/**
uint32_t texture_sample(Texture* texture, Vec2 pixel, Vec4 a, Vec4 b, Vec4 c, Vec2 a_uv, Vec2 b_uv, Vec2 c_uv, float reciprocal_area) {
  Vec2 AC, AP, PB, PC;
  xy_sub(c, a, AC);
  xy_sub(pixel, a, AP);
  xy_sub(b, pixel, PB);
  xy_sub(c, pixel, PC);

  const float weight_a = (PC.x * PB.y - PC.y * PB.x) * reciprocal_area;
  const float weight_b = (AC.x * AP.y - AC.y * AP.x) * reciprocal_area;
  const float weight_c = 1.0f - weight_a - weight_b;

  const float interpolated_w = 1.0f / (a.w * weight_a + b.w * weight_b + c.w * weight_c);

  const float s = a_uv.x * a.w * weight_a + b_uv.x * b.w * weight_b + c_uv.x * c.w * weight_c;
  const float t = a_uv.y * a.w * weight_a + b_uv.y * b.w * weight_b + c_uv.y * c.w * weight_c;

  int x = (int)(fabsf(s * interpolated_w) * texture->width);
  int y = (int)(fabsf(t * interpolated_w) * texture->height);

  if (x >= texture->width) {
    x = texture->width - 1;
  }

  if (y >= texture->height) {
    y = texture->height - 1;
  }

  return texture->bitmap[texture->width * x + y];
}
**/
