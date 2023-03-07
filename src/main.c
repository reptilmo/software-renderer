// main.c
#include "darray.h"
#include "display.h"
#include "mat.h"
#include "mesh.h"
#include "renderer.h"
#include "texture.h"
#include "config.h"

void process_input(bool* running);
void update(float delta_time);
void render(void);

Display* display = NULL;
Renderer* renderer = NULL;
Texture* texture = NULL;

bool enable_backface_culling = true;
bool enable_draw_fill = false;
bool enable_draw_wireframe = false;
bool enable_draw_points = false;
bool enable_texture_mapping = true;
DrawMode draw_mode = DRAW_MODE_TEXTURE;
LightMode light_mode = LIGHT_MODE_FLAT;

Vec3 camera_postion = {0.0f, 0.0f, -5.0f};

Mesh* mesh = NULL;
Vec3 mesh_rotation = {.x = 0, .y = 0, .z = 0};

Vec3* transformed_mesh_vertices = NULL;
Vec3* transformed_mesh_normals = NULL;

int main(int argc, char* argv[]) {
  int rval = 0;
  Config config;
  process_command_line(&config, argc, argv);

  if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) != 0) {
    fprintf(stderr, "Error from SDL init!\n");
    return 1;
  }

  display = init_display(config.window_width, config.window_height, config.full_screen);
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
  renderer_cull_mode(renderer, CULL_MODE_BACKFACE);
  renderer_light_mode(renderer, light_mode);

  bool mesh_loaded = false;
  if (config.mesh_file != NULL) {
    mesh_loaded = mesh_load_obj(mesh, config.mesh_file);
  } else {
    mesh_loaded = mesh_load_cube(mesh);
  }

  bool texture_loaded = false;
  if (config.texture_file != NULL) {
    texture_loaded = texture_load_tga(texture, config.texture_file);
  } else {
    texture_loaded = texture_load_checker_board(texture);
  }


  bool running = mesh_loaded && texture_loaded;
  int previous_frame_time = 0;
  int delta_time = 0;

  while (running) {
    process_input(&running);

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
    } else if (event.key.keysym.scancode == SDL_SCANCODE_4) {
      if (enable_texture_mapping) {
        enable_texture_mapping = false;
        draw_mode &= ~DRAW_MODE_TEXTURE;
      } else {
        enable_texture_mapping = true;
        draw_mode |= DRAW_MODE_TEXTURE;
      }
      renderer_draw_mode(renderer, draw_mode);
    }

    break;
  default:
    break;
  }
}

void update(float dt) {
  ASSERT(mesh != NULL);

  mesh_rotation.x += 0.01f * dt;
  mesh_rotation.y += 0.01f * dt;
  mesh_rotation.z += 0.01f * dt;

  camera_postion.z += sinf(mesh_rotation.x) * 0.04f;

  Mat4 rotate = mat4_mul(mat4_make_rotate_z(mesh_rotation.z),
                         mat4_mul(mat4_make_rotate_y(mesh_rotation.y),
                                  mat4_make_rotate_x(mesh_rotation.x)));

  Mat4 translate = mat4_make_translate(0.0f, 0.0f, 0.0f);

  const Vec3 camera_target = {0.0f, 0.0f, 5.0f};
  const Vec3 world_up = {0.0f, 1.0f, 0.0f};

  Mat4 view = mat4_make_look_at(&camera_postion, &camera_target, &world_up);

  Mat4 transform = mat4_mul(view, mat4_mul(translate, rotate));

  const size_t vertex_count = mesh_vertex_count(mesh);
  const size_t normal_count = mesh_normal_count(mesh);

  dyn_array_clear(transformed_mesh_vertices);
  dyn_array_clear(transformed_mesh_normals);

  for (int i = 0; i < vertex_count; i++) {
    Vec3 vertex = mesh->vertices[i];
    Vec4 transformed = mat4_mul_vec4(transform, vec3_xyzw(&vertex));
    vertex = vec4_xyz(&transformed);

    dyn_array_push_back(transformed_mesh_vertices, vertex);
  }

  for (int i = 0; i < normal_count; i++) {
    Vec3 normal = mesh->normals[i];
    //FIXME: I think there is a problem here.
    Vec4 transformed = mat4_mul_vec4(rotate, vec3_xyzw(&normal));
    normal = vec4_xyz(&transformed);

    dyn_array_push_back(transformed_mesh_normals, normal);
  }
}

void render(void) {
  ASSERT(renderer != NULL);
  renderer_begin_frame(renderer);

  renderer_camera_position(renderer, &camera_postion);
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
