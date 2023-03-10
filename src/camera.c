// camera.c
#include "camera.h"

#define VERTICAL_MAX 90.0f

void init_camera(Camera* camera, Vec3 postion, Vec3 forward, Vec3 world_up) {
  ASSERT(camera != NULL);
  camera->position = postion;
  camera->forward = forward;
  camera->world_up = world_up;
  camera->vertical_angle = 0.0f;
  camera->horizontal_angle = 90.0f;
}

void camera_update(Camera* camera, const InputHandler* input, float delta_time) {
  ASSERT(camera != NULL);

  const float dy = mouse_delta_y(input) * delta_time;
  const float dx = mouse_delta_x(input) * delta_time;

  // camera->vertical_angle += dy;
  camera->horizontal_angle += dx;

  printf("-> %d %d - %f %f\n", input->delta_x, input->delta_y, camera->horizontal_angle, camera->vertical_angle);

  if (camera->vertical_angle > VERTICAL_MAX) {
    camera->vertical_angle = VERTICAL_MAX;
  } else if (camera->vertical_angle < -VERTICAL_MAX) {
    camera->vertical_angle = -VERTICAL_MAX;
  }

  const float vertical_cos = cosf(RADIANS(camera->vertical_angle));
  const float vertical_sin = sinf(RADIANS(camera->vertical_angle));
  const float horizontal_cos = cosf(RADIANS(camera->horizontal_angle));
  const float horizontal_sin = sinf(RADIANS(camera->horizontal_angle));

  camera->forward.x = vertical_cos * horizontal_cos;
  camera->forward.y = vertical_sin;
  camera->forward.z = vertical_cos * horizontal_sin;
  vec3_normalize(&camera->forward);

  float move_factor = 0.01f;
  Vec3 scaled_forward = vec3_mul(&camera->forward, move_factor);

  if (move_forward(input)) {
    camera->position = vec3_add(&camera->position, &scaled_forward);
  }

  if (move_back(input)) {
    camera->position = vec3_sub(&camera->position, &scaled_forward);
  }

  if (move_left(input)) {
    Vec3 direction = vec3_cross(&camera->world_up, &camera->forward);
    direction = vec3_mul(&direction, move_factor);
    camera->position = vec3_add(&camera->position, &direction);
  }

  if (move_right(input)) {
    Vec3 direction = vec3_cross(&camera->world_up, &camera->forward);
    direction = vec3_mul(&direction, move_factor);
    camera->position = vec3_sub(&camera->position, &direction);
  }

  if (move_up(input)) {
    camera->position.y += move_factor;
  }

  if (move_down(input)) {
    camera->position.y -= move_factor;
  }
}

Mat4 camera_view_matrix(const Camera* camera) {
  ASSERT(camera != NULL);

  Vec3 right = vec3_cross(&camera->forward, &camera->world_up);
  vec3_normalize(&right);
  Vec3 up = vec3_cross(&right, &camera->forward);

  return (Mat4){{
      {right.x, right.y, right.z, -vec3_dot(&right, &camera->position)},
      {up.x, up.y, up.z, -vec3_dot(&up, &camera->position)},
      {camera->forward.x, camera->forward.y, camera->forward.z, -vec3_dot(&camera->forward, &camera->position)},
      {0.0f, 0.0f, 0.0f, 1.0f},
  }};
}
