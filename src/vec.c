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
    .y = v.y * cos(angle) - v.z * sin(angle),
    .z = v.z * cos(angle) + v.y * sin(angle),
  };
  return rotated;
}

Vec3 rotate_around_y(Vec3 v, float angle) {
  Vec3 rotated = {
    .x = v.x * cos(angle) - v.z * sin(angle),
    .y = v.y,
    .z = v.z * cos(angle) + v.x * sin(angle),
  };
  return rotated;
}

Vec3 rotate_around_z(Vec3 v, float angle) {
  Vec3 rotated = {
    .x = v.x * cos(angle) - v.y * sin(angle),
    .y = v.y * cos(angle) + v.x * sin(angle),
    .z = v.z,
  };
  return rotated;
}

