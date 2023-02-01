// main.c
#include <stdio.h>

#include "display.h"
#include "vec.h"

bool setup(void);
void process_input(bool* running);
void update(void);
void render(void);
Vec2 project(Vec3 point);

Display display = {0};

#define CUBE_POINTS (9 * 9 * 9)

Vec3 cube[CUBE_POINTS];
Vec2 cube_projected[CUBE_POINTS];

Vec3 cube_rotation = {.x = 0, .y = 0, .z = 0};
Vec3 camera_position = {.x = 0, .y = 0, .z = -5};


int main(int argc, char* argv[]) {

  if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) != 0) {
    fprintf(stderr, "Error from SDL init!\n");
    return 1;
  }

  if (!initialize_display(&display, 800, 600, false)) {
    return 1;
  }

  bool running = setup();

  while (running) {
    process_input(&running);
    update();
    render();
  }

  finalize_display(&display);
  SDL_Quit();

  return 0;
}

bool setup(void) {

  int point_index = 0;
  for (float x = -1; x <= 1; x += 0.25) {
    for (float y = -1; y <= 1; y += 0.25) {
      for (float z = -1; z <= 1; z += 0.25) {
        Vec3 v = {.x = x, .y = y, .z = z};
        cube[point_index++] = v;
      }
    }
  }

  return true;
}

void process_input(bool* running) {
  SDL_Event event;
  SDL_PollEvent(&event);

  switch (event.type) {
  case SDL_QUIT:
    *running = false;
    break;
  case SDL_KEYDOWN:
    if (event.key.keysym.scancode == SDL_SCANCODE_ESCAPE) {
      *running = false;
    }
    break;
  default:
    break;
  }
}

void update(void) {
  cube_rotation.x += 0.001f;
  cube_rotation.y += 0.001f;
  cube_rotation.z += 0.001f;

  for (int i = 0; i < CUBE_POINTS; i++) {
    Vec3 p = cube[i];
    p = rotate_around_x(p, cube_rotation.x);
    p = rotate_around_y(p, cube_rotation.y);
    p = rotate_around_z(p, cube_rotation.z);

    p = sub(p, camera_position); // TODO:

    cube_projected[i] = project(p);
  }
}

Vec2 project(Vec3 point) {
  const float SCALE = 640;

  Vec2 projected = {
      .x = SCALE * point.x / point.z,
      .y = SCALE * point.y / point.z,
  };

  return projected;
}

void render(void) {
  clear_pixel_buffer(&display, 0xFF000000);
  draw_grid(&display, 10, 0xFF333333);

  // draw_rect(&display, 50, 50, 20, 30, 0xFFFF0000);
  // draw_rect(&display, 300, 200, 70, 40, 0xFFFFFF00);

  for (int i = 0; i < CUBE_POINTS; i++) {
    draw_rect(
        &display,
        (int)cube_projected[i].x + display.width / 2,
        (int)cube_projected[i].y + display.height / 2,
        4,
        4,
        0xFF00FF33);
  }

  present_pixel_buffer(&display);
}
