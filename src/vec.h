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

INLINE void vec2_swap(Vec2* a, Vec2* b) {
  Vec2 tmp = {.x = a->x, .y = a->y};
  *a = *b;
  *b = tmp;
}

INLINE void vec4_swap(Vec4* a, Vec4* b) {
  Vec4 tmp = {
      .x = a->x,
      .y = a->y,
      .z = a->z,
      .w = a->w,
  };
  *a = *b;
  *b = tmp;
}

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

INLINE float vec2_len(const Vec2* v) {
  return sqrtf(v->x * v->x + v->y * v->y);
}

INLINE float vec2_dot(Vec2 a, Vec2 b) {
  return a.x * b.x + a.y * b.y;
}

INLINE Vec2 vec2_normalized(const Vec2* v) {
  const float factor = 1.0f / vec2_len(v);
  return (Vec2){
      .x = v->x * factor,
      .y = v->y * factor,
  };
}

INLINE Vec3 vec3_add(const Vec3* a, const Vec3* b) {
  return (Vec3){
      .x = a->x + b->x,
      .y = a->y + b->y,
      .z = a->z + b->z,
  };
}

INLINE Vec3 vec3_sub(const Vec3* a, const Vec3* b) {
  return (Vec3){
      .x = a->x - b->x,
      .y = a->y - b->y,
      .z = a->z - b->z,
  };
}

INLINE Vec3 vec3_mul_vec3(const Vec3* a, const Vec3* b) {
  return (Vec3){
      .x = a->x * b->x,
      .y = a->y * b->y,
      .z = a->z * b->z,
  };
}

INLINE Vec3 vec3_mul(const Vec3* v, float s) {
  return (Vec3){
      .x = v->x * s,
      .y = v->y * s,
      .z = v->z * s,
  };
}

INLINE Vec3 vec3_div(const Vec3* v, float s) {
  return (Vec3){
      .x = v->x / s,
      .y = v->y / s,
      .z = v->z / s,
  };
}

INLINE Vec3 vec3_lerp(const Vec3* start, const Vec3* end, float t) {
  return (Vec3){
      .x = start->x + t * (end->x - start->x),
      .y = start->y + t * (end->y - start->y),
      .z = start->z + t * (end->z - start->z),
  };
}

INLINE float vec3_len(const Vec3* v) {
  return sqrtf(v->x * v->x + v->y * v->y + v->z * v->z);
}

INLINE Vec3 vec3_cross(Vec3 a, Vec3 b) {
  return (Vec3){
      .x = a.y * b.z - a.z * b.y,
      .y = a.z * b.x - a.x * b.z,
      .z = a.x * b.y - b.y * b.x,
  };
}

INLINE float vec3_dot(Vec3 a, Vec3 b) {
  return a.x * b.x + a.y * b.y + a.z * b.z;
}

INLINE Vec3 vec3_normalize(const Vec3* v) {
  const float factor = 1.0f / vec3_len(v);
  return (Vec3){
      .x = v->x * factor,
      .y = v->y * factor,
      .z = v->z * factor,
  };
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

INLINE Vec2 vec4_xy(Vec4 v) {
  return *(Vec2*)(&v);
  /*Vec2 out = {
      .x = v.x,
      .y = v.y,
  };
  return out;*/
}

INLINE Vec3 vec4_xyz(Vec4 v) {
  return (Vec3){
      .x = v.x,
      .y = v.y,
      .z = v.z,
  };
}

INLINE Vec4 vec3_xyzw(Vec3 v) {
  return (Vec4){
      .x = v.x,
      .y = v.y,
      .z = v.z,
      1,
  };
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

// FIXME:
#define xy_sub(a, b, c)    \
  {                        \
    (c).x = (a).x - (b).x; \
    (c).y = (a).y - (b).y; \
  }
