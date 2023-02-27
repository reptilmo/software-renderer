// triangle.h
#pragma once

#include "vec.h"

typedef struct Triangle {
  Vec4 points[3];
  Vec2 uvs[3];
  float light_intensity;
} Triangle;
