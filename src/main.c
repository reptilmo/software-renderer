// main.c
#include <stdio.h>
#include <assert.h>

#include "display.h"
#include "vec.h"
#include "mesh.h"
#include "darray.h"
#include "renderer.h"

#define FPS 60
#define FRAME_TARGET_TIME (1000 / FPS)

bool setup(void);
void teardown(void);
void process_input(bool* running);
void update(void);
void render(void);

Display display = {0};
Renderer* renderer = NULL;
const Vec3 camera_position = {.x = 0, .y = 0, .z = 0};
bool enable_backface_culling = false;

Mesh* mesh = NULL;
Vec3 mesh_rotation = {.x = 0, .y = 0, .z = 0};
Vec3* mesh_normals = NULL;

Vec3* transformed_mesh_vertices = NULL;
Vec3* transformed_mesh_normals = NULL;

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

  teardown();

  finalize_display(&display);
  SDL_Quit();

  return 0;
}

bool setup(void) {

  mesh = init_mesh();
  if (mesh != NULL) {
    //FIXEME:
    load_cube_mesh(mesh);
    /*if (!load_obj_mesh(mesh, "E:\\software-renderer\\data\\torus.obj")) {
      return false;
    }*/
  } else {
    return false;
  }

  renderer = init_renderer(&display);
  if (renderer != NULL) {
    renderer_camera_position(renderer, camera_position);
    renderer_clear_color(renderer, 0xFF000000);
  } else {
    return false;
  }

  return true;
}

void teardown(void) {

  destroy_renderer(renderer);
  renderer = NULL;

  dyn_array_free(transformed_mesh_vertices);

  if (mesh != NULL) {
    dyn_array_free(mesh->vertices);
    dyn_array_free(mesh->triangles);
    free(mesh);
  }
}

void process_input(bool* running) {
  SDL_Event event;
  SDL_PollEvent(&event);

  switch (event.type) {
  case SDL_QUIT:
    *running = false;
    break;
  case SDL_KEYUP:
    if (event.key.keysym.scancode == SDL_SCANCODE_ESCAPE) {
      *running = false;
    } else if (event.key.keysym.scancode == SDL_SCANCODE_TAB) {
      if (enable_backface_culling) {
        enable_backface_culling = false;
      } else {
        enable_backface_culling = true;
      }
      renderer_backface_culling(renderer, enable_backface_culling);
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

  assert(mesh != NULL);
  const size_t mesh_vertex_count = get_mesh_vertex_count(mesh);
  const size_t mesh_normal_count = get_mesh_normal_count(mesh);

  dyn_array_clear(transformed_mesh_vertices);

  for (int i = 0; i < mesh_vertex_count; i++) {
    Vec3 vertex = mesh->vertices[i];
    vertex = rotate_around_x(vertex, mesh_rotation.x);
    vertex = rotate_around_y(vertex, mesh_rotation.y);
    vertex = rotate_around_z(vertex, mesh_rotation.z);
    vertex.z += 5.0f;

    dyn_array_push_back(transformed_mesh_vertices, vertex);
  }

  dyn_array_clear(transformed_mesh_normals);

  for (int i = 0; i < mesh_normal_count; i++) {
    Vec3 normal = mesh->normals[i];
    normal = rotate_around_x(normal, mesh_rotation.x);
    normal = rotate_around_y(normal, mesh_rotation.y);
    normal = rotate_around_z(normal, mesh_rotation.z);

    dyn_array_push_back(transformed_mesh_normals, normal);
  }
}

void render(void) {
  assert(renderer != NULL);

  renderer_begin_frame(renderer);

  renderer_begin_triangles(
    renderer,
    mesh->triangles,
    dyn_array_length(mesh->triangles),
    transformed_mesh_vertices,
    dyn_array_length(transformed_mesh_vertices),
    transformed_mesh_normals,
    dyn_array_length(transformed_mesh_normals)
  );

  renderer_end_triangles(renderer);
  renderer_end_frame(renderer);
}
