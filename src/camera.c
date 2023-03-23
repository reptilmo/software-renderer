// camera.c
#include "camera.h"

#define VERTICAL_MAX 90.0f

void init_camera(Camera* camera, Vec3 postion, Vec3 world_up) {
  ASSERT(camera != NULL);
  camera->position = postion;
  camera->world_up = world_up;
  camera->vertical_angle = 0.0f;
  camera->horizontal_angle = 0.0f;
}

Mat4 camera_update_view_matrix(Camera* camera, const InputHandler* input, float delta_time) {
  ASSERT(camera != NULL);

  const float dy = 0.1f * mouse_delta_y(input) * delta_time;
  const float dx = 0.1f * mouse_delta_x(input) * delta_time;

  camera->vertical_angle -= dy;
  camera->horizontal_angle += dx;

  //if (camera->vertical_angle > VERTICAL_MAX) {
  //  camera->vertical_angle = VERTICAL_MAX;
  //} else if (camera->vertical_angle < -VERTICAL_MAX) {
  //  camera->vertical_angle = -VERTICAL_MAX;
  //}

  const float cos_yaw = cosf(RADIANS(camera->horizontal_angle));
  const float sin_yaw = sinf(RADIANS(camera->horizontal_angle));
  const float cos_pitch = cosf(RADIANS(camera->vertical_angle));
  const float sin_pitch = sinf(RADIANS(camera->vertical_angle));

  const Vec3 x_axis = {cos_yaw, sin_pitch * sin_yaw, -cos_pitch * sin_yaw};
  const Vec3 y_axis = {0, cos_pitch, sin_pitch};
  const Vec3 z_axis = {sin_yaw, -cos_yaw * sin_pitch, cos_pitch * cos_yaw};

  const float move_factor = 0.01f * delta_time;

  Vec3 forward = {x_axis.z, y_axis.z, z_axis.z};
  vec3_normalize(&forward);
  forward = vec3_mul(&forward, move_factor);

  Vec3 right = {x_axis.x, y_axis.x, z_axis.x};
  vec3_normalize(&right);
  right = vec3_mul(&right, move_factor);

  if (move_forward(input)) {
    camera->position = vec3_add(&camera->position, &forward);
  }

  if (move_back(input)) {
    camera->position = vec3_sub(&camera->position, &forward);
  }

  if (move_right(input)) {
    camera->position = vec3_add(&camera->position, &right);
  }

  if (move_left(input)) {
    camera->position = vec3_sub(&camera->position, &right);
  }

  if (move_up(input)) {
    camera->position.y += move_factor;
  }

  if (move_down(input)) {
    camera->position.y -= move_factor;
  }

  const Mat4 rotate = {{
      x_axis.x, y_axis.x, z_axis.x, 0,
      x_axis.y, y_axis.y, z_axis.y, 0,
      x_axis.z, y_axis.z, z_axis.z, 0,
      0,   0,   0,   1,
  }};

  return mat4_mul(rotate, mat4_make_translate(-camera->position.x, -camera->position.y, -camera->position.z));
}
