// color.h
#pragma once

#include "system.h"

INLINE uint32_t color_apply_intensity(uint32_t color, float intencity) {
  if (intencity < 0.0f) {
    intencity = 0.0f;
  }

  const uint32_t a = (color & 0xFF000000) >> 24;
  const uint32_t r = (color & 0x00FF0000) >> 16;
  const uint32_t g = (color & 0x0000FF00) >> 8;
  const uint32_t b = (color & 0x000000FF) >> 0;

  return ((uint32_t)(a * intencity) << 24) |
         ((uint32_t)(r * intencity) << 16) |
         ((uint32_t)(g * intencity) << 8) |
         ((uint32_t)(b * intencity) << 0);
}
