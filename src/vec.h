// vec.h
#pragma once

#include "system.h"

typedef struct Vec2 {
  float x, y;
} Vec2;

typedef struct Vec3 {
  float x, y, z;
} Vec3;

typedef struct Vec4 {
  float x, y, z, w;
} Vec4;


INLINE Vec2 vec2_add(Vec2 a, Vec2 b) {
  Vec2 result = {
      .x = a.x + b.x,
      .y = a.y + b.y,
  };
  return result;
}

INLINE Vec2 vec2_sub(Vec2 a, Vec2 b) {
  Vec2 result = {
      .x = a.x - b.x,
      .y = a.y - b.y,
  };
  return result;
}

INLINE Vec2 vec2_mul(Vec2 v, float s) {
  Vec2 result = {
      .x = v.x * s,
      .y = v.y * s,
  };
  return result;
}

INLINE Vec2 vec2_div(Vec2 v, float s) {
  Vec2 result = {
      .x = v.x / s,
      .y = v.y / s,
  };
  return result;
}

INLINE float vec2_len(Vec2 v) {
  return sqrtf(v.x * v.x + v.y * v.y);
}

INLINE float vec2_dot(Vec2 a, Vec2 b) {
  return a.x * b.x + a.y * b.y;
}

INLINE Vec2 vec2_normalize(Vec2 v) {
  const float factor = 1.0f / vec2_len(v);
  Vec2 result = {
      .x = v.x * factor,
      .y = v.y * factor,
  };
  return result;
}

INLINE Vec3 vec3_add(Vec3 a, Vec3 b) {
  Vec3 result = {
      .x = a.x + b.x,
      .y = a.y + b.y,
      .z = a.z + b.z,
  };
  return result;
}

INLINE Vec3 vec3_sub(Vec3 a, Vec3 b) {
  Vec3 result = {
      .x = a.x - b.x,
      .y = a.y - b.y,
      .z = a.z - b.z,
  };
  return result;
}

INLINE Vec3 vec3_mul(Vec3 v, float s) {
  Vec3 result = {
      .x = v.x * s,
      .y = v.y * s,
      .z = v.z * s,
  };
  return result;
}

INLINE Vec3 vec3_div(Vec3 v, float s) {
  Vec3 result = {
      .x = v.x / s,
      .y = v.y / s,
      .z = v.z / s,
  };
  return result;
}

INLINE float vec3_len(Vec3 v) {
  return sqrtf(v.x * v.x + v.y * v.y + v.z * v.z);
}

INLINE Vec3 vec3_cross(Vec3 a, Vec3 b) {
  Vec3 result = {
      .x = a.y * b.z - a.z * b.y,
      .y = a.z * b.x - a.x * b.z,
      .z = a.x * b.y - b.y * b.x,
  };
  return result;
}

INLINE float vec3_dot(Vec3 a, Vec3 b) {
  return a.x * b.x + a.y * b.y + a.z * b.z;
}

INLINE Vec3 vec3_normalize(Vec3 v) {
  const float factor = 1.0f / vec3_len(v);
  Vec3 result = {
      .x = v.x * factor,
      .y = v.y * factor,
      .z = v.z * factor,
  };
  return result;
}

INLINE Vec3 rotate_around_x(Vec3 v, float a) {
  const float cos_a = cosf(a);
  const float sin_a = sinf(a);
  Vec3 rotated = {
      .x = v.x,
      .y = v.y * cos_a - v.z * sin_a,
      .z = v.z * cos_a + v.y * sin_a,
  };
  return rotated;
}

INLINE Vec3 rotate_around_y(Vec3 v, float a) {
  const float cos_a = cosf(a);
  const float sin_a = sinf(a);
  Vec3 rotated = {
      .x = v.x * cos_a - v.z * sin_a,
      .y = v.y,
      .z = v.z * cos_a + v.x * sin_a,
  };
  return rotated;
}

INLINE Vec3 rotate_around_z(Vec3 v, float a) {
  const float cos_a = cosf(a);
  const float sin_a = sinf(a);
  Vec3 rotated = {
      .x = v.x * cos_a - v.y * sin_a,
      .y = v.y * cos_a + v.x * sin_a,
      .z = v.z,
  };
  return rotated;
}

INLINE Vec3 vec4_xyz(Vec4 v) {
  Vec3 out = {
    .x = v.x,
    .y = v.y,
    .z = v.z,
  };
  return out;
}

INLINE Vec4 vec3_xyzw(Vec3 v) {
  Vec4 out = {
    .x = v.x,
    .y = v.y,
    .z = v.z,
    1,
  };
  return out;
}

INLINE Vec4 perspective_divide(Vec4 p) {
  if (p.w != 0.0) {
    const float w_factor = 1.0f / p.w;
    p.x *= w_factor;
    p.y *= w_factor;
    p.z *= w_factor;
  }

  return p;
}
