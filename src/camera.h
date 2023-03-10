// camera.h
#pragma once

#include "input.h"
#include "mat.h"
#include "vec.h"

typedef struct Camera {
  Vec3 position;
  Vec3 forward;
  Vec3 world_up;
  float vertical_angle;
  float horizontal_angle;
} Camera;

void init_camera(Camera* camera, Vec3 postion, Vec3 forward, Vec3 world_up);
void camera_update(Camera* camera, const InputHandler* input, float delta_time);
Mat4 camera_view_matrix(const Camera* camera);
