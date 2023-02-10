// main.c
#include "darray.h"
#include "display.h"
#include "mesh.h"
#include "renderer.h"
#include "vec.h"
#include "mat.h"

#define FPS 60
#define FRAME_TARGET_TIME (1000 / FPS)

const char* process_command_line(int argc, char* argv[], int* width, int* height, bool* fullscreen);
void process_input(bool* running);
void update(void);
void render(void);

Display* display = NULL;
Renderer* renderer = NULL;
const Vec3 camera_position = {.x = 0, .y = 0, .z = 0};
bool enable_backface_culling = true;
bool enable_draw_fill = true;
bool enable_draw_wireframe = true;
bool enable_draw_points = false;
DrawMode draw_mode = DRAW_MODE_TRIANGLE_FILL | DRAW_MODE_TRIANGLE_WIRE;

Mesh* mesh = NULL;
float mesh_scale = 1.0f;
Vec3 mesh_rotation = {.x = 0, .y = 0, .z = 0};


Vec3* transformed_mesh_vertices = NULL;
Vec3* transformed_mesh_normals = NULL;
uint32_t previous_frame_time = 0;

int main(int argc, char* argv[]) {

  int width = 800;
  int height = 600;
  bool fullscreen = false;
  const char* mesh_file = process_command_line(argc, argv, &width, &height, &fullscreen);

  if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) != 0) {
    fprintf(stderr, "Error from SDL init!\n");
    return 1;
  }

  display = init_display(width, height, fullscreen);
  if (display == NULL) {
    SDL_Quit();
    return 1;
  }

  renderer = init_renderer(display);
  if (renderer == NULL) {
    destroy_display(display);
    SDL_Quit();
    return 1;
  }

  renderer_camera_position(renderer, camera_position);
  renderer_clear_color(renderer, 0xFF000000);
  renderer_draw_mode(renderer, draw_mode);
  renderer_cull_mode(renderer, CULL_MODE_BACKFACE);

  mesh = init_mesh();
  if (mesh == NULL) {
    destroy_renderer(renderer);
    destroy_display(display);
    SDL_Quit();
    return 1;
  }

  bool running = false;
  if (mesh_file != NULL) {
    running = load_obj_mesh(mesh, mesh_file);
  } else {
    running = load_cube_mesh(mesh);
  }

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

  destroy_mesh(mesh);
  destroy_renderer(renderer);
  destroy_display(display);
  SDL_Quit();

  return 0;
}

const char* process_command_line(int argc, char* argv[], int* width, int* height, bool* fullscreen) {

  int requested_width = 0;
  int requested_height = 0;
  bool requested_fullscreen = false;
  const char* requested_mesh_file = NULL;

  for (int i = 1; i < argc; i++) {
    if (strcmp(argv[i], "--fullscreen") == 0) {
      requested_fullscreen = true;
    } else if (sscanf(argv[i], "--w%d", &requested_width) == 1) {
      continue;
    } else if (sscanf(argv[i], "--h%d", &requested_height) == 1) {
      continue;
    } else if (strncmp(argv[i], "--mesh=", 7) == 0) {
      requested_mesh_file = argv[i] + 7;
    }
  }

  if (requested_width > 0 && requested_height > 0) {
    *width = requested_width;
    *height = requested_height;
  }

  if (requested_fullscreen) {
    *fullscreen = requested_fullscreen;
  }

  return requested_mesh_file;
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
      renderer_cull_mode(renderer,
                         enable_backface_culling ? CULL_MODE_BACKFACE : CULL_MODE_NONE);
    } else if (event.key.keysym.scancode == SDL_SCANCODE_1) {
      if (enable_draw_fill) {
        enable_draw_fill = false;
        draw_mode &= ~DRAW_MODE_TRIANGLE_FILL;
      } else {
        enable_draw_fill = true;
        draw_mode |= DRAW_MODE_TRIANGLE_FILL;
      }
      renderer_draw_mode(renderer, draw_mode);
    } else if (event.key.keysym.scancode == SDL_SCANCODE_2) {
      if (enable_draw_wireframe) {
        enable_draw_wireframe = false;
        draw_mode &= ~DRAW_MODE_TRIANGLE_WIRE;
      } else {
        enable_draw_wireframe = true;
        draw_mode |= DRAW_MODE_TRIANGLE_WIRE;
      }
      renderer_draw_mode(renderer, draw_mode);
    } else if (event.key.keysym.scancode == SDL_SCANCODE_3) {
      if (enable_draw_points) {
        enable_draw_points = false;
        draw_mode &= ~DRAW_MODE_POINTS;
      } else {
        enable_draw_points = true;
        draw_mode |= DRAW_MODE_POINTS;
      }
      renderer_draw_mode(renderer, draw_mode);
    }
    break;
  default:
    break;
  }
}

void update(void) {
  assert(mesh != NULL);

  mesh_scale += 0.001f;
  if (mesh_scale >= 2.0f) {
    mesh_scale = 1.0f;
  }

  mesh_rotation.x += 0.01f;
  mesh_rotation.y += 0.01f;
  mesh_rotation.z += 0.01f;

  Mat4 scale = mat4_make_scale(mesh_scale, mesh_scale, mesh_scale);

  Mat4 rotate = mat4_mul(mat4_make_rotate_z(mesh_rotation.z),
    mat4_mul(mat4_make_rotate_y(mesh_rotation.y),
      mat4_make_rotate_x(mesh_rotation.x)));

  Mat4 translate = mat4_make_translate(0.0f, 0.0f, 10.0f);
  Mat4 transform = mat4_mul(mat4_mul(translate, rotate), scale);

  const size_t mesh_vertex_count = get_mesh_vertex_count(mesh);
  const size_t mesh_normal_count = get_mesh_normal_count(mesh);

  dyn_array_clear(transformed_mesh_vertices);

  for (int i = 0; i < mesh_vertex_count; i++) {
    Vec3 vertex = mesh->vertices[i];
    vertex = vec4_xyz(mat4_mul_vec4(transform, vec3_xyzw(vertex)));

    dyn_array_push_back(transformed_mesh_vertices, vertex);
  }

  dyn_array_clear(transformed_mesh_normals);

  for (int i = 0; i < mesh_normal_count; i++) {
    Vec3 normal = vec4_xyz(mat4_mul_vec4(
      rotate,
      vec3_xyzw(mesh->normals[i])
    ));

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
      dyn_array_length(transformed_mesh_normals));

  renderer_end_triangles(renderer);
  renderer_end_frame(renderer);
}
