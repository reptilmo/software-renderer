// vec.c
#include "vec.h"

#include <math.h>

Vec3 add(Vec3 a, Vec3 b) {
  Vec3 c = {
    .x = a.x + b.x,
    .y = a.y + b.y,
    .z = a.z + b.z,
  };
  return c;
}

Vec3 sub(Vec3 a, Vec3 b) {
  Vec3 c = {
    .x = a.x - b.x,
    .y = a.y - b.y,
    .z = a.z - b.z,
  };
  return c;
}

Vec3 mul(Vec3 v, float s) {
  Vec3 c = {
    .x = v.x * s,
    .y = v.y * s,
    .z = v.z * s,
  };
  return c;
}

Vec3 rotate_around_x(Vec3 v, float angle) {
  Vec3 rotated = {
    .x = v.x,
    .y = v.y * (float)cos(angle) - v.z * (float)sin(angle),
    .z = v.z * (float)cos(angle) + v.y * (float)sin(angle),
  };
  return rotated;
}

Vec3 rotate_around_y(Vec3 v, float angle) {
  Vec3 rotated = {
    .x = v.x * (float)cos(angle) - v.z * (float)sin(angle),
    .y = v.y,
    .z = v.z * (float)cos(angle) + v.x * (float)sin(angle),
  };
  return rotated;
}

Vec3 rotate_around_z(Vec3 v, float angle) {
  Vec3 rotated = {
    .x = v.x * (float)cos(angle) - v.y * (float)sin(angle),
    .y = v.y * (float)cos(angle) + v.x * (float)sin(angle),
    .z = v.z,
  };
  return rotated;
}

Vec2 project(Vec3 point) {
  const float SCALE = 640;

  Vec2 projected = {
      .x = SCALE * point.x / point.z,
      .y = SCALE * point.y / point.z,
  };

  return projected;
}

