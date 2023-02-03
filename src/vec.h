// vec.h
#pragma once

typedef struct Vec2 {
  float x;
  float y;
} Vec2;

typedef struct Vec3 {
  float x;
  float y;
  float z;
} Vec3;

Vec3 add(Vec3 a, Vec3 b);
Vec3 sub(Vec3 a, Vec3 b);
Vec3 mul(Vec3 v, float s);

Vec3 rotate_around_x(Vec3 v, float a);
Vec3 rotate_around_y(Vec3 v, float a);
Vec3 rotate_around_z(Vec3 v, float a);

Vec2 project(Vec3 point);

