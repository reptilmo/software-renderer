// mat.h
#pragma once

#include "system.h"
#include "vec.h"

typedef struct Mat4 {
  float m[4][4];
} Mat4;

INLINE Mat4 mat4_identity(void) {
  return (Mat4){{
      {1, 0, 0, 0},
      {0, 1, 0, 0},
      {0, 0, 1, 0},
      {0, 0, 0, 1},
  }};
}

INLINE Mat4 mat4_mul(Mat4 a, Mat4 b) {
  return (Mat4){{{
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
}

INLINE Vec4 mat4_mul_vec4(Mat4 m, Vec4 v) {
  return (Vec4){
      m.m[0][0] * v.x + m.m[0][1] * v.y + m.m[0][2] * v.z + m.m[0][3] * v.w,
      m.m[1][0] * v.x + m.m[1][1] * v.y + m.m[1][2] * v.z + m.m[1][3] * v.w,
      m.m[2][0] * v.x + m.m[2][1] * v.y + m.m[2][2] * v.z + m.m[2][3] * v.w,
      m.m[3][0] * v.x + m.m[3][1] * v.y + m.m[3][2] * v.z + m.m[3][3] * v.w,
  };
}

INLINE Mat4 mat4_make_scale(float sx, float sy, float sz) {
  return (Mat4){{
      {sx, 0, 0, 0},
      {0, sy, 0, 0},
      {0, 0, sz, 0},
      {0, 0, 0, 1},
  }};
}

INLINE Mat4 mat4_make_translate(float tx, float ty, float tz) {
  return (Mat4){{
      {1, 0, 0, tx},
      {0, 1, 0, ty},
      {0, 0, 1, tz},
      {0, 0, 0, 1},
  }};
}

INLINE Mat4 mat4_make_rotate_x(float a) {
  const float cos_a = cosf(a);
  const float sin_a = sinf(a);
  return (Mat4){{
      {1, 0, 0, 0},
      {0, cos_a, -sin_a, 0},
      {0, sin_a, cos_a, 0},
      {0, 0, 0, 1},
  }};
}

INLINE Mat4 mat4_make_rotate_y(float a) {
  const float cos_a = cosf(a);
  const float sin_a = sinf(a);
  return (Mat4){{
      {cos_a, 0, -sin_a, 0},
      {0, 1, 0, 0},
      {sin_a, 0, cos_a, 0},
      {0, 0, 0, 1},
  }};
}

INLINE Mat4 mat4_make_rotate_z(float a) {
  const float cos_a = cosf(a);
  const float sin_a = sinf(a);
  return (Mat4){{
      {cos_a, -sin_a, 0, 0},
      {sin_a, cos_a, 0, 0},
      {0, 0, 1, 0},
      {0, 0, 0, 1},
  }};
}

INLINE Mat4 mat4_make_look_at(const Vec3* position, const Vec3* target, const Vec3* world_up) {
  // Left-handed
  Vec3 forward = vec3_sub(target, position);
  vec3_normalize(&forward);
  Vec3 right = vec3_cross(world_up, &forward);
  vec3_normalize(&right);
  Vec3 up = vec3_cross(&forward, &right);

  return (Mat4){{
      {right.x, right.y, right.z, -vec3_dot(&right, position)},
      {up.x, up.y, up.z, -vec3_dot(&up, position)},
      {forward.x, forward.y, forward.z, -vec3_dot(&forward, position)},
      {0, 0, 0, 1},
  }};
}

INLINE Mat4 mat4_make_perspective(float view_width,
                                  float view_height, float fov_over_two, float near_plane, float far_plane) {

  const float aspect_ratio = view_height / view_width;
  const float fov_factor = 1.0f / tanf(RADIANS(fov_over_two));
  const float far_factor = far_plane / (far_plane - near_plane);
  const float near_factor = -1.0f * far_factor * near_plane;

  return (Mat4){{
      {aspect_ratio * fov_factor, 0, 0, 0},
      {0, fov_factor, 0, 0},
      {0, 0, far_factor, near_factor},
      {0, 0, 1, 0},
  }};
}
