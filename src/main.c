// main.c
#include <stdio.h>

#include "display.h"
#include "vec.h"
#include "mesh.h"

#define FPS 60
#define FRAME_TARGET_TIME (1000 / FPS)

bool setup(void);
void process_input(bool* running);
void update(void);
void render(void);
Vec2 project(Vec3 point);

Display display = {0};

Vec2 mesh_vertices_projected[NUM_MESH_VERTICES];

Vec3 mesh_rotation = {.x = 0, .y = 0, .z = 0};
Vec3 camera_position = {.x = 0, .y = 0, .z = -5};

uint32_t previous_frame_time = 0;

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

    int wait_next_frame = FRAME_TARGET_TIME - (SDL_GetTicks() - previous_frame_time);

    if (wait_next_frame > 0 && wait_next_frame <= FRAME_TARGET_TIME) {
      SDL_Delay((uint32_t)wait_next_frame);
    }
    previous_frame_time = SDL_GetTicks();

    update();
    render();
  }

  finalize_display(&display);
  SDL_Quit();

  return 0;
}

bool setup(void) {

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
  mesh_rotation.x += 0.01f;
  mesh_rotation.y += 0.01f;
  mesh_rotation.z += 0.01f;

  for (int i = 0; i < NUM_MESH_VERTICES; i++) {
    Vec3 p = mesh_vertices[i];
    p = rotate_around_x(p, mesh_rotation.x);
    p = rotate_around_y(p, mesh_rotation.y);
    p = rotate_around_z(p, mesh_rotation.z);

    p = sub(p, camera_position); // TODO:

    mesh_vertices_projected[i] = project(p);
  }
}

void render(void) {
  clear_pixel_buffer(&display, 0xFF000000);
  draw_grid(&display, 10, 0xFF333333);

  // draw_rect(&display, 50, 50, 20, 30, 0xFFFF0000);
  // draw_rect(&display, 300, 200, 70, 40, 0xFFFFFF00);

  for (int i = 0; i < NUM_MESH_TRIANGLES; i++) {
    Vec2 a = mesh_vertices_projected[mesh_triangles[i].a];
    Vec2 b = mesh_vertices_projected[mesh_triangles[i].b];
    Vec2 c = mesh_vertices_projected[mesh_triangles[i].c];

    //FIXME:
    a.x += display.width / 2;
    a.y += display.height / 2;
    b.x += display.width / 2;
    b.y += display.height / 2;
    c.x += display.width / 2;
    c.y += display.height / 2;

    draw_rect(&display, a.x, a.y, 10, 10, 0xFFFFFF00);
    draw_rect(&display, b.x, b.y, 10, 10, 0xFFFFFF00);
    draw_rect(&display, c.x, c.y, 10, 10, 0xFFFFFF00);

    draw_line_dda(&display, a.x, a.y, b.x, b.y, 0xFFFF0000);
    draw_line_dda(&display, b.x, b.y, c.x, c.y, 0xFF0000FF);
    draw_line_dda(&display, c.x, c.y, a.x, a.y, 0xFF00FF00);
  }

  present_pixel_buffer(&display);
}
