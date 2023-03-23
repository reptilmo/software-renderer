// main.c
#include "darray.h"
#include "display.h"
#include "mat.h"
#include "mesh.h"
#include "renderer.h"
#include "texture.h"
#include "config.h"
#include "input.h"
#include "camera.h"

void update(float delta_time);
void render(void);

ConfigMap config;
InputHandler input;
Display* display = NULL;
Renderer* renderer = NULL;
Texture* texture = NULL;
Camera camera;

DrawMode draw_mode = DRAW_MODE_TEXTURE;
LightMode light_mode = LIGHT_MODE_FLAT;

Vec3 camera_position = {0.0f, 0.0f, 0.0f};

Mesh* mesh = NULL;
Vec3 mesh_rotation = {.x = 0, .y = 0, .z = 0};

Vec3* transformed_mesh_vertices = NULL;
Vec3* transformed_mesh_normals = NULL;

int main(int argc, char* argv[]) {
  int rval = 0;

  init_config_map(&config);
  process_command_line(&config, argc, argv);
  init_input_handler(&input, &config);

  const bool fullscreen = config_map_value_int(&config, "fullscreen", 0) > 0;
  const int window_width = config_map_value_int(&config, "window_width", 800);
  const int window_height = config_map_value_int(&config, "window_height", 600);
  const char* mesh_file_path = config_map_value_str(&config, "load_mesh", NULL);
  const char* texture_file_path = config_map_value_str(&config, "load_texture", NULL);

  if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) != 0) {
    fprintf(stderr, "Error from SDL init!\n");
    return 1;
  }

  display = init_display(window_width, window_height, fullscreen);
  if (display == NULL) {
    rval = 1;
    goto EXIT;
  }

  renderer = init_renderer(display);
  if (renderer == NULL) {
    rval = 1;
    goto EXIT;
  }

  mesh = init_mesh();
  if (mesh == NULL) {
    rval = 1;
    goto EXIT;
  }

  texture = init_texture();
  if (texture == NULL) {
    rval = 1;
    goto EXIT;
  }

  //FIXME: Move initial state out of here.
  renderer_clear_color(renderer, 0xFF000000);
  renderer_draw_mode(renderer, draw_mode);
  renderer_cull_mode(renderer, CULL_MODE_FRONT_FACE);
  //renderer_light_mode(renderer, light_mode);

  bool mesh_loaded = false;
  if (mesh_file_path != NULL) {
    mesh_loaded = mesh_load_obj(mesh, mesh_file_path);
  } else {
    mesh_loaded = mesh_load_cube(mesh);
  }

  bool texture_loaded = false;
  if (texture_file_path != NULL) {
    texture_loaded = texture_load_tga(texture, texture_file_path);
  } else {
    texture_loaded = texture_load_checker_board(texture);
  }

  bool running = mesh_loaded && texture_loaded;
  uint64_t previous_frame_time = 0;
  uint64_t delta_time = 0;

  const Vec3 world_up = {0.0f, 1.0f, 0.0f};
  init_camera(&camera, camera_position, world_up);

  while (running) {
    SDL_PumpEvents();
    running = input_handler_update(&input, display);

    delta_time = SDL_GetTicks() - previous_frame_time;
    previous_frame_time = SDL_GetTicks();

    update(delta_time / 10.f);
    render();
  }

EXIT:
  destroy_texture(texture);
  destroy_mesh(mesh);
  destroy_renderer(renderer);
  destroy_display(display);
  SDL_Quit();

  return rval;
}

//FIXME:
Vec3 translation = {0.0f, 0.0f, 10.0f};

void update(float dt) {
  ASSERT(mesh != NULL);

  //mesh_rotation.x += 0.01f * dt;
  //mesh_rotation.y += 0.01f * dt;
  //mesh_rotation.z += 0.01f * dt;

 /* Mat4 rotate = mat4_mul(mat4_make_rotate_z(mesh_rotation.z),
                         mat4_mul(mat4_make_rotate_y(mesh_rotation.y),
                                  mat4_make_rotate_x(mesh_rotation.x)));*/

  Mat4 translate = mat4_make_translate(translation.x,translation.y, translation.z);

  camera_update_view_matrix(&camera, &input, dt);
  Mat4 view = camera_update_view_matrix(&camera, &input, dt);
  //Mat4 transform = mat4_mul(rotate, view);
  Mat4 transform = view;

  const size_t vertex_count = mesh_vertex_count(mesh);
  const size_t normal_count = mesh_normal_count(mesh);

  dyn_array_clear(transformed_mesh_vertices);
  dyn_array_clear(transformed_mesh_normals);

  for (int i = 0; i < vertex_count; i++) {
    Vec4 vertex = vec3_xyzw(&mesh->vertices[i]);
    vertex = mat4_mul_vec4(&transform, &vertex);
    dyn_array_push_back(transformed_mesh_vertices, vec4_xyz(&vertex));
  }

  for (int i = 0; i < normal_count; i++) {
    Vec4 normal = vec3_xyzw(&mesh->normals[i]);
    //FIXME: I think there is a problem here.
    normal = mat4_mul_vec4(&transform, &normal);
    dyn_array_push_back(transformed_mesh_normals, vec4_xyz(&normal));
  }
}

void render(void) {
  ASSERT(renderer != NULL);
  renderer_begin_frame(renderer);

  renderer_camera_position(renderer, &camera_position);
  renderer_current_texture(renderer, texture);
  renderer_begin_triangles(
      renderer,
      mesh->triangles,
      mesh_triangle_count(mesh),
      transformed_mesh_vertices,
      dyn_array_length(transformed_mesh_vertices),
      transformed_mesh_normals,
      dyn_array_length(transformed_mesh_normals),
      mesh->uvs, mesh_uv_count(mesh));

  renderer_end_triangles(renderer);
  renderer_end_frame(renderer);
}
