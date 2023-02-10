// mat.h
#pragma once

#include "system.h"
#include "vec.h"

typedef struct Mat4 {
  float m[4][4];
} Mat4;

INLINE Mat4 mat4_identity(void) {
  Mat4 m = {{
      {1, 0, 0, 0},
      {0, 1, 0, 0},
      {0, 0, 1, 0},
      {0, 0, 0, 1},
  }};
  return m;
}

INLINE Mat4 mat4_mul(Mat4 a, Mat4 b) {
  Mat4 product = {{{
    a.m[0][0] * b.m[0][0] + a.m[0][1] * b.m[1][0] + a.m[0][2] * b.m[2][0] + a.m[0][3] * b.m[3][0],
    a.m[0][0] * b.m[0][1] + a.m[0][1] * b.m[1][1] + a.m[0][2] * b.m[2][1] + a.m[0][3] * b.m[3][1],
    a.m[0][0] * b.m[0][2] + a.m[0][1] * b.m[1][2] + a.m[0][2] * b.m[2][2] + a.m[0][3] * b.m[3][2],
    a.m[0][0] * b.m[0][3] + a.m[0][1] * b.m[1][3] + a.m[0][3] * b.m[2][3] + a.m[0][3] * b.m[3][3],
  },{
    a.m[1][0] * b.m[0][0] + a.m[1][1] * b.m[1][0] + a.m[1][2] * b.m[2][0] + a.m[1][3] * b.m[3][0],
    a.m[1][0] * b.m[0][1] + a.m[1][1] * b.m[1][1] + a.m[1][2] * b.m[2][1] + a.m[1][3] * b.m[3][1],
    a.m[1][0] * b.m[0][2] + a.m[1][1] * b.m[1][2] + a.m[1][2] * b.m[2][2] + a.m[1][3] * b.m[3][2],
    a.m[1][0] * b.m[0][3] + a.m[1][1] * b.m[1][3] + a.m[1][3] * b.m[2][3] + a.m[1][3] * b.m[3][3],
  },{
    a.m[2][0] * b.m[0][0] + a.m[2][1] * b.m[1][0] + a.m[2][2] * b.m[2][0] + a.m[2][3] * b.m[3][0],
    a.m[2][0] * b.m[0][1] + a.m[2][1] * b.m[1][1] + a.m[2][2] * b.m[2][1] + a.m[2][3] * b.m[3][1],
    a.m[2][0] * b.m[0][2] + a.m[2][1] * b.m[1][2] + a.m[2][2] * b.m[2][2] + a.m[2][3] * b.m[3][2],
    a.m[2][0] * b.m[0][3] + a.m[2][1] * b.m[1][3] + a.m[2][3] * b.m[2][3] + a.m[2][3] * b.m[3][3],
  },{
    a.m[3][0] * b.m[0][0] + a.m[3][1] * b.m[1][0] + a.m[3][2] * b.m[2][0] + a.m[3][3] * b.m[3][0],
    a.m[3][0] * b.m[0][1] + a.m[3][1] * b.m[1][1] + a.m[3][2] * b.m[2][1] + a.m[3][3] * b.m[3][1],
    a.m[3][0] * b.m[0][2] + a.m[3][1] * b.m[1][2] + a.m[3][2] * b.m[2][2] + a.m[3][3] * b.m[3][2],
    a.m[3][0] * b.m[0][3] + a.m[3][1] * b.m[1][3] + a.m[3][3] * b.m[2][3] + a.m[3][3] * b.m[3][3],
  }}};
  return product;
}

INLINE Vec4 mat4_mul_vec4(Mat4 m, Vec4 v) {
  Vec4 product = {
      m.m[0][0] * v.x + m.m[0][1] * v.y + m.m[0][2] * v.z + m.m[0][3] * v.w,
      m.m[1][0] * v.x + m.m[1][1] * v.y + m.m[1][2] * v.z + m.m[1][3] * v.w,
      m.m[2][0] * v.x + m.m[2][1] * v.y + m.m[2][2] * v.z + m.m[2][3] * v.w,
      m.m[3][0] * v.x + m.m[3][1] * v.y + m.m[3][2] * v.z + m.m[3][3] * v.w,
  };
  return product;
}

INLINE Mat4 mat4_make_scale(float sx, float sy, float sz) {
  Mat4 m = {{
      {sx, 0, 0, 0},
      {0, sy, 0, 0},
      {0, 0, sz, 0},
      {0, 0, 0, 1},
  }};
  return m;
}

INLINE Mat4 mat4_make_translate(float tx, float ty, float tz) {
  Mat4 m = {{
      {1, 0, 0, tx},
      {0, 1, 0, ty},
      {0, 0, 1, tz},
      {0, 0, 0, 1},
  }};
  return m;
}

INLINE Mat4 mat4_make_rotate_x(float a) {
  const float cos_a = cosf(a);
  const float sin_a = sinf(a);
  Mat4 m = {{
      {1, 0, 0, 0},
      {0, cos_a, -sin_a, 0},
      {0, sin_a, cos_a, 0},
      {0, 0, 0, 1},
  }};
  return m;
}

INLINE Mat4 mat4_make_rotate_y(float a) {
  const float cos_a = cosf(a);
  const float sin_a = sinf(a);
  Mat4 m = {{
      {cos_a, 0, -sin_a, 0},
      {0, 1, 0, 0},
      {sin_a, 0, cos_a, 0},
      {0, 0, 0, 1},
  }};
  return m;
}

INLINE Mat4 mat4_make_rotate_z(float a) {
  const float cos_a = cosf(a);
  const float sin_a = sinf(a);
  Mat4 m = {{
      {cos_a, -sin_a, 0, 0},
      {sin_a, cos_a, 0, 0},
      {0, 0, 1, 0},
      {0, 0, 0, 1},
  }};
  return m;
}
