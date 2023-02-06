// vec.c
#include "vec.h"

#include <math.h>

Vec2 vec2_add(Vec2 a, Vec2 b) {
  Vec2 result = {
    .x = a.x + b.x,
    .y = a.y + b.y,
  };
  return result;
}

Vec2 vec2_sub(Vec2 a, Vec2 b) {
  Vec2 result = {
    .x = a.x - b.x,
    .y = a.y - b.y,
  };
  return result;
}

Vec2 vec2_mul(Vec2 v, float s) {
  Vec2 result = {
    .x = v.x * s,
    .y = v.y * s,
  };
  return result;
}

Vec2 vec2_div(Vec2 v, float s) {
    Vec2 result = {
    .x = v.x / s,
    .y = v.y / s,
  };
  return result;
}

float vec2_len(Vec2 v) {
  return sqrtf(v.x * v.x + v.y * v.y);
}

float vec2_dot(Vec2 a, Vec2 b) {
  return a.x * b.x + a.y * b.y;
}

Vec3 vec3_add(Vec3 a, Vec3 b) {
  Vec3 result = {
    .x = a.x + b.x,
    .y = a.y + b.y,
    .z = a.z + b.z,
  };
  return result;
}

Vec3 vec3_sub(Vec3 a, Vec3 b) {
  Vec3 result = {
    .x = a.x - b.x,
    .y = a.y - b.y,
    .z = a.z - b.z,
  };
  return result;
}

Vec3 vec3_mul(Vec3 v, float s) {
  Vec3 result = {
    .x = v.x * s,
    .y = v.y * s,
    .z = v.z * s,
  };
  return result;
}

Vec3 vec3_div(Vec3 v, float s) {
  Vec3 result = {
    .x = v.x / s,
    .y = v.y / s,
    .z = v.z / s,
  };
  return result;
}

float vec3_len(Vec3 v) {
  return sqrtf(v.x * v.x + v.y * v.y + v.z * v.z);
}

Vec3 vec3_cross(Vec3 a, Vec3 b) {
  Vec3 result = {
    .x = a.y * b.z - a.z * b.y,
    .y = a.z * b.x - a.x * b.z,
    .z = a.x * b.y - b.y * b.x,
  };
  return result;
}

float vec3_dot(Vec3 a, Vec3 b) {
  return a.x * b.x + a.y * b.y + a.z * b.z;
}

Vec3 rotate_around_x(Vec3 v, float angle) {
  Vec3 rotated = {
    .x = v.x,
    .y = v.y * cosf(angle) - v.z * sinf(angle),
    .z = v.z * cosf(angle) + v.y * sinf(angle),
  };
  return rotated;
}

Vec3 rotate_around_y(Vec3 v, float angle) {
  Vec3 rotated = {
    .x = v.x * cosf(angle) - v.z * sinf(angle),
    .y = v.y,
    .z = v.z * cosf(angle) + v.x * sinf(angle),
  };
  return rotated;
}

Vec3 rotate_around_z(Vec3 v, float angle) {
  Vec3 rotated = {
    .x = v.x * cosf(angle) - v.y * sinf(angle),
    .y = v.y * cosf(angle) + v.x * sinf(angle),
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

