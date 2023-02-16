// color.h
#pragma once

#include "system.h"

typedef struct Color {
  uint8_t a, r, g, b;
} Color;

INLINE Color color_from_u32(uint32_t c) {
  Color out = {
      .a = (uint8_t)((c & 0xFF000000) >> 24),
      .r = (uint8_t)((c & 0x00FF0000) >> 16),
      .g = (uint8_t)((c & 0x0000FF00) >> 8),
      .b = (uint8_t)((c & 0x000000FF) >> 0),
  };
  return out;
}

INLINE uint32_t color_to_u32(Color c) {
  return (c.a << 24) | (c.r << 16) | (c.g << 8) | (c.b << 0);
}

INLINE Color color_apply_intensity(Color c, float intensity) {
  Color out = {
      .a = c.a,
      .r = (uint8_t)(c.r * intensity),
      .g = (uint8_t)(c.g * intensity),
      .b = (uint8_t)(c.b * intensity),
  };
  return out;
}
