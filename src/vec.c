// vec.c
#include "vec.h"


Vec2 project(Vec3 point) {
  const float SCALE = 640;

  Vec2 projected = {
      .x = SCALE * point.x / point.z,
      .y = SCALE * point.y / point.z,
  };

  return projected;
}
