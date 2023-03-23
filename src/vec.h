// vec.h
#pragma once

#include "system.h"

typedef enum Axis {
  X_AXIS = 0,
  Y_AXIS = 1,
  Z_AXIS = 2,
  W_AXIS = 3
} Axis;

typedef struct Vec2 {
  float x, y;
} Vec2;

typedef struct Vec3 {
  float x, y, z;
} Vec3;

typedef struct Vec4 {
  float x, y, z, w;
} Vec4;

inline void vec2_swap(Vec2* a, Vec2* b) {
  Vec2 tmp = {.x = a->x, .y = a->y};
  *a = *b;
  *b = tmp;
}

inline void vec4_swap(Vec4* a, Vec4* b) {
  Vec4 tmp = {
      .x = a->x,
      .y = a->y,
      .z = a->z,
      .w = a->w,
  };
  *a = *b;
  *b = tmp;
}

inline Vec3 vec3_new(float x, float y, float z) {
  return (Vec3){.x = x, .y = y, .z = z};
}

inline Vec2 vec2_add(const Vec2* a, const Vec2* b) {
  return (Vec2){
      .x = a->x + b->x,
      .y = a->y + b->y,
  };
}

inline Vec2 vec2_sub(const Vec2* a, const Vec2* b) {
  return (Vec2){
      .x = a->x - b->x,
      .y = a->y - b->y,
  };
}

inline Vec2 vec2_mul(const Vec2* v, float s) {
  return (Vec2){
      .x = v->x * s,
      .y = v->y * s,
  };
}

inline Vec2 vec2_div(const Vec2* v, float s) {
  return (Vec2){
      .x = v->x / s,
      .y = v->y / s,
  };
}

inline float vec2_len(const Vec2* v) {
  return sqrtf(v->x * v->x + v->y * v->y);
}

inline float vec2_dot(const Vec2* a, const Vec2* b) {
  return a->x * b->x + a->y * b->y;
}

inline Vec2 vec2_normalized(const Vec2* v) {
  const float factor = 1.0f / vec2_len(v);
  return (Vec2){
      .x = v->x * factor,
      .y = v->y * factor,
  };
}

inline Vec2 vec2_lerp(const Vec2* start, const Vec2* end, float t) {
  return (Vec2){
      .x = start->x + t * (end->x - start->x),
      .y = start->y + t * (end->y - start->y),
  };
}

inline Vec3 vec3_add(const Vec3* a, const Vec3* b) {
  return (Vec3){
      .x = a->x + b->x,
      .y = a->y + b->y,
      .z = a->z + b->z,
  };
}

inline Vec3 vec3_sub(const Vec3* a, const Vec3* b) {
  return (Vec3){
      .x = a->x - b->x,
      .y = a->y - b->y,
      .z = a->z - b->z,
  };
}

inline Vec3 vec3_mul_vec3(const Vec3* a, const Vec3* b) {
  return (Vec3){
      .x = a->x * b->x,
      .y = a->y * b->y,
      .z = a->z * b->z,
  };
}

inline Vec3 vec3_mul(const Vec3* v, float s) {
  return (Vec3){
      .x = v->x * s,
      .y = v->y * s,
      .z = v->z * s,
  };
}

inline Vec3 vec3_div(const Vec3* v, float s) {
  return (Vec3){
      .x = v->x / s,
      .y = v->y / s,
      .z = v->z / s,
  };
}

inline Vec3 vec3_lerp(const Vec3* start, const Vec3* end, float t) {
  return (Vec3){
      .x = start->x + t * (end->x - start->x),
      .y = start->y + t * (end->y - start->y),
      .z = start->z + t * (end->z - start->z),
  };
}

inline float vec3_len(const Vec3* v) {
  return sqrtf(v->x * v->x + v->y * v->y + v->z * v->z);
}

inline Vec3 vec3_cross(const Vec3* a, const Vec3* b) {
  return (Vec3){
      .x = (a->y * b->z) - (a->z * b->y),
      .y = (a->z * b->x) - (a->x * b->z),
      .z = (a->x * b->y) - (b->y * b->x),
  };
}

inline float vec3_dot(const Vec3* a, const Vec3* b) {
  return a->x * b->x + a->y * b->y + a->z * b->z;
}

inline Vec3 vec3_normalized(const Vec3* v) {
  const float factor = 1.0f / vec3_len(v);
  return (Vec3){
      .x = v->x * factor,
      .y = v->y * factor,
      .z = v->z * factor,
  };
}

inline void vec3_normalize(Vec3* v) {
  const float factor = 1.0f / vec3_len(v);
  v->x *= factor;
  v->y *= factor;
  v->z *= factor;
}

inline Vec3 rotate_around_x(const Vec3* v, float a) {
  const float cos_a = cosf(a);
  const float sin_a = sinf(a);
  return (Vec3){
      .x = v->x,
      .y = v->y * cos_a - v->z * sin_a,
      .z = v->z * cos_a + v->y * sin_a,
  };
}

inline Vec3 rotate_around_y(const Vec3* v, float a) {
  const float cos_a = cosf(a);
  const float sin_a = sinf(a);
  return (Vec3){
      .x = v->x * cos_a - v->z * sin_a,
      .y = v->y,
      .z = v->z * cos_a + v->x * sin_a,
  };
}

inline Vec3 rotate_around_z(const Vec3* v, float a) {
  const float cos_a = cosf(a);
  const float sin_a = sinf(a);
  return (Vec3){
      .x = v->x * cos_a - v->y * sin_a,
      .y = v->y * cos_a + v->x * sin_a,
      .z = v->z,
  };
}

inline Vec3 vec4_xyz(const Vec4* v) {
  return (Vec3){
      .x = v->x,
      .y = v->y,
      .z = v->z,
  };
}

inline Vec4 vec3_xyzw(const Vec3* v) {
  return (Vec4){
      .x = v->x,
      .y = v->y,
      .z = v->z,
      1,
  };
}

inline float vec4_axis(const Vec4* v, Axis axis) {
  switch (axis) {
  case X_AXIS:
    return v->x;
  case Y_AXIS:
    return v->y;
  case Z_AXIS:
    return v->z;
  case W_AXIS:
    return v->w;
  default:
    ASSERT(0);
    return 0.0f;
  }
}

inline Vec4 vec4_lerp(const Vec4* start, const Vec4* end, float t) {
  return (Vec4){
      .x = start->x + t * (end->x - start->x),
      .y = start->y + t * (end->y - start->y),
      .z = start->z + t * (end->z - start->z),
      .w = start->w + t * (end->w - start->w),
  };
}

inline Vec4 perspective_divide(const Vec4* p) {
  ASSERT(p->w != 0.0f)
  const float w_factor = 1.0f / p->w;
  return (Vec4){
      .x = p->x * w_factor,
      .y = p->y * w_factor,
      .z = p->z * w_factor,
      .w = p->w,
  };
}
