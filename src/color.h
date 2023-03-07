// color.h
#pragma once

#include "system.h"

INLINE uint32_t color_apply_intensity(uint32_t color, float intencity) {
  if (intencity < 0.0f) {
    intencity = 0.0f;
  }

  const uint32_t a = (color & 0xFF000000) >> 24;
  uint32_t r = (color & 0x00FF0000) >> 16;
  uint32_t g = (color & 0x0000FF00) >> 8;
  uint32_t b = (color & 0x000000FF) >> 0;

  // r = (uint32_t)(((r / 255.0f) * intencity) * 255.0f);
  // g = (uint32_t)(((g / 255.0f) * intencity) * 255.0f);
  // b = (uint32_t)(((b / 255.0f) * intencity) * 255.0f);

  //FIXME: Doing this per pixel seems to currently seems to be a big performance hit!
  r = (uint32_t)(r * intencity);
  g = (uint32_t)(g * intencity);
  b = (uint32_t)(b * intencity);

  return a << 24 | r << 16 | g << 8 | b;
}
