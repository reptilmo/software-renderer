// vec.h
#pragma once

typedef struct Vec2 {
  float x, y;
} Vec2;

typedef struct Vec3 {
  float x, y, z;
} Vec3;

Vec2 vec2_add(Vec2 a, Vec2 b);
Vec2 vec2_sub(Vec2 a, Vec2 b);
Vec2 vec2_mul(Vec2 v, float s);
Vec2 vec2_div(Vec2 v, float s);
float vec2_len(Vec2 v);
float vec2_dot(Vec2 a, Vec2 b);
Vec2 vec2_normalize(Vec2 v);

Vec3 vec3_add(Vec3 a, Vec3 b);
Vec3 vec3_sub(Vec3 a, Vec3 b);
Vec3 vec3_mul(Vec3 v, float s);
Vec3 vec3_div(Vec3 v, float s);
float vec3_len(Vec3 v);
Vec3 vec3_cross(Vec3 a, Vec3 b);
float vec3_dot(Vec3 a, Vec3 b);
Vec3 vec3_normalize(Vec3 v);

Vec3 rotate_around_x(Vec3 v, float a);
Vec3 rotate_around_y(Vec3 v, float a);
Vec3 rotate_around_z(Vec3 v, float a);

Vec2 project(Vec3 point); // FIXME:

