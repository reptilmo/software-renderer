// mat.h
#pragma once

#include "system.h"
#include "vec.h"

// Column matrix
// col_1.x, col_1.y, col_1.z, 0
// col_2.x, col_2.y, col_2.z, 0
// col_3.x, col_3.y, col_3.z, 0
//       0,       0,       0, 1
typedef struct Mat4 {
  float m[16];
} Mat4;

inline Mat4 mat4_identity(void) {
  return (Mat4){{
      1, 0, 0, 0,
      0, 1, 0, 0,
      0, 0, 1, 0,
      0, 0, 0, 1,
  }};
}

inline Mat4 mat4_mul(Mat4 l, Mat4 r) {
  return (Mat4){{
      l.m[0] * r.m[0] + l.m[1] * r.m[4] + l.m[2] * r.m[8] + l.m[3] * r.m[12],
      l.m[0] * r.m[1] + l.m[1] * r.m[5] + l.m[2] * r.m[9] + l.m[3] * r.m[13],
      l.m[0] * r.m[2] + l.m[1] * r.m[6] + l.m[2] * r.m[10] + l.m[3] * r.m[14],
      l.m[0] * r.m[3] + l.m[1] * r.m[7] + l.m[2] * r.m[11] + l.m[3] * r.m[15],

      l.m[4] * r.m[0] + l.m[5] * r.m[4] + l.m[6] * r.m[8] + l.m[7] * r.m[12],
      l.m[4] * r.m[1] + l.m[5] * r.m[5] + l.m[6] * r.m[9] + l.m[7] * r.m[13],
      l.m[4] * r.m[2] + l.m[5] * r.m[6] + l.m[6] * r.m[10] + l.m[7] * r.m[14],
      l.m[4] * r.m[3] + l.m[5] * r.m[7] + l.m[6] * r.m[11] + l.m[7] * r.m[15],

      l.m[8] * r.m[0] + l.m[9] * r.m[4] + l.m[10] * r.m[8] + l.m[11] * r.m[12],
      l.m[8] * r.m[1] + l.m[9] * r.m[5] + l.m[10] * r.m[9] + l.m[11] * r.m[13],
      l.m[8] * r.m[2] + l.m[9] * r.m[6] + l.m[10] * r.m[10] + l.m[11] * r.m[14],
      l.m[8] * r.m[3] + l.m[9] * r.m[7] + l.m[10] * r.m[11] + l.m[11] * r.m[15],

      l.m[12] * r.m[0] + l.m[13] * r.m[4] + l.m[14] * r.m[8] + l.m[15] * r.m[12],
      l.m[12] * r.m[1] + l.m[13] * r.m[5] + l.m[14] * r.m[9] + l.m[15] * r.m[13],
      l.m[12] * r.m[2] + l.m[13] * r.m[6] + l.m[14] * r.m[10] + l.m[15] * r.m[14],
      l.m[12] * r.m[3] + l.m[13] * r.m[7] + l.m[14] * r.m[11] + l.m[15] * r.m[15],
  }};
}

inline Vec4 mat4_mul_vec4(const Mat4* m, const Vec4* v) {
  return (Vec4) {
      m->m[0]  * v->x + m->m[1]  * v->y + m->m[2]  * v->z + m->m[3]  * v->w,
      m->m[4]  * v->x + m->m[5]  * v->y + m->m[6]  * v->z + m->m[7]  * v->w,
      m->m[8]  * v->x + m->m[9]  * v->y + m->m[10] * v->z + m->m[11] * v->w,
      m->m[12] * v->x + m->m[13] * v->y + m->m[14] * v->z + m->m[15] * v->w,
  };
}

inline Mat4 mat4_make_translate(float tx, float ty, float tz) {
  return (Mat4){{
    1, 0, 0, tx,
    0, 1, 0, ty,
    0, 0, 1, tz,
    0, 0, 0, 1,
  }};
}

inline Mat4 mat4_make_scale(float sx, float sy, float sz) {
  return (Mat4){{
    sx,   0,   0,  0,
     0,  sy,   0,  0,
     0,   0,  sz,  0,
     0,   0,   0,  1,
  }};
}

//inline Mat4 mat4_make_rotate_x(float a) {
//  const float cos_a = cosf(a);
//  const float sin_a = sinf(a);
//  return (Mat4){{
//      {1, 0, 0, 0},
//      {0, cos_a, -sin_a, 0},
//      {0, sin_a, cos_a, 0},
//      {0, 0, 0, 1},
//  }};
//}
//
//inline Mat4 mat4_make_rotate_y(float a) {
//  const float cos_a = cosf(a);
//  const float sin_a = sinf(a);
//  return (Mat4){{
//      {cos_a, 0, -sin_a, 0},
//      {0, 1, 0, 0},
//      {sin_a, 0, cos_a, 0},
//      {0, 0, 0, 1},
//  }};
//}
//
//inline Mat4 mat4_make_rotate_z(float a) {
//  const float cos_a = cosf(a);
//  const float sin_a = sinf(a);
//  return (Mat4){{
//      {cos_a, -sin_a, 0, 0},
//      {sin_a, cos_a, 0, 0},
//      {0, 0, 1, 0},
//      {0, 0, 0, 1},
//  }};
//}
//


inline Mat4 mat4_make_perspective(float view_width,
                                  float view_height, float fov_over_two, float near_plane, float far_plane) {

  const float aspect_ratio = view_height / view_width;
  const float fov_factor = 1.0f / tanf(RADIANS(fov_over_two));
  const float far_factor = (far_plane + near_plane) / (far_plane - near_plane);
  const float near_factor = (-2.0f * far_plane * near_plane) / (far_plane - near_plane);

  return (Mat4){{
      aspect_ratio * fov_factor, 0, 0, 0,
      0, fov_factor, 0, 0,
      0, 0, far_factor, near_factor,
      0, 0, 1, 0,
  }};
}
