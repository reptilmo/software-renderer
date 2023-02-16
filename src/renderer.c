// renderer.c
#include "renderer.h"
#include "darray.h"
#include "sort.h"
#include "color.h"

void swap_render_triangles(void* left, void* right) {
  Triangle tmp = *(Triangle*)left;
  *(Triangle*)left = *(Triangle*)right;
  *(Triangle*)right = tmp;
}

bool farther_triangle(void* left, void* right) {

  const float left_avg_z = (((Triangle*)left)->a.z + ((Triangle*)left)->b.z + ((Triangle*)left)->c.z) / 3.0f;

  const float right_avg_z = (((Triangle*)right)->a.z + ((Triangle*)right)->b.z + ((Triangle*)right)->c.z) / 3.0f;

  return left_avg_z > right_avg_z;
}

Renderer* init_renderer(Display* display) {
  ASSERT(display != NULL);
  if (display != NULL) {
    Renderer* renderer = (Renderer*)malloc(sizeof(Renderer));
    if (renderer != NULL) {
      renderer->display = display;
      renderer->renderable_triangles = NULL;
      renderer->current_texture = NULL;
      renderer->camera_position.x = 0.0f;
      renderer->camera_position.y = 0.0f;
      renderer->camera_position.z = 0.0f;
      renderer->light_direction.x = 0.0f;
      renderer->light_direction.y = 0.0f;
      renderer->light_direction.z = 1.0f;
      renderer->clear_color = 0xFF000000;
      renderer->draw_mode = DRAW_MODE_TRIANGLE_FILL;
      renderer->cull_mode = CULL_MODE_NONE;

      renderer->projection_matrix = mat4_make_perspective(
        (float)renderer->display->width,
        (float)renderer->display->height,
        60.0f, 1.0f, 1000.0f);

      renderer->view_half_width = display->width * 0.5f;
      renderer->view_half_height = display->height * 0.5f;

      return renderer;
    }
  }

  return NULL;
}

void destroy_renderer(Renderer* renderer) {
  ASSERT(renderer != NULL);
  if (renderer != NULL) {
    dyn_array_free(renderer->renderable_triangles);
    free(renderer);
  }
}

void renderer_cull_mode(Renderer* renderer, CullMode cull_mode) {
  ASSERT(renderer != NULL);
  if (renderer != NULL) {
    renderer->cull_mode = cull_mode;
  }
}

void renderer_draw_mode(Renderer* renderer, DrawMode draw_mode) {
  ASSERT(renderer != NULL);
  if (renderer != NULL) {
    renderer->draw_mode = draw_mode;
  }
}

void renderer_camera_position(Renderer* renderer, Vec3 position) {
  ASSERT(renderer != NULL);
  if (renderer != NULL) {
    renderer->camera_position = position;
  }
}

void renderer_clear_color(Renderer* renderer, uint32_t color) {
  ASSERT(renderer != NULL);
  if (renderer != NULL) {
    renderer->clear_color = color;
  }
}

void renderer_current_texture(Renderer* renderer, Texture* texture) {
  ASSERT(renderer != NULL);
  if (renderer != NULL) {
    renderer->current_texture = texture;
  }
}

void renderer_begin_frame(Renderer* renderer) {
  ASSERT(renderer != NULL);

  clear_pixel_buffer(renderer->display, renderer->clear_color);
  draw_grid(renderer->display, 10, 0xFF333333);
}

void renderer_end_frame(Renderer* renderer) {
  ASSERT(renderer != NULL);

  present_pixel_buffer(renderer->display);
}

void renderer_begin_triangles(Renderer* renderer, TriangleFace* faces, size_t num_faces,
  Vec3* vertices, size_t num_vertices, Vec3* normals, size_t num_normals, Vec2* uvs, size_t num_uvs) {

  ASSERT(renderer != NULL);

  for (size_t face_idx = 0; face_idx < num_faces; face_idx++) {

    const TriangleFace face = faces[face_idx];
    ASSERT(face.a < num_vertices);
    ASSERT(face.b < num_vertices);
    ASSERT(face.c < num_vertices);

    ASSERT(face.a_uv < num_uvs);
    ASSERT(face.b_uv < num_uvs);
    ASSERT(face.c_uv < num_uvs);

    /*const Vec3 ab = vec3_normalize(vec3_sub(triangle.b, triangle.a));
      const Vec3 ac = vec3_normalize(vec3_sub(triangle.c, triangle.a));
      const Vec3 triangle_normal = vec3_cross(ab, ac);*/
    ASSERT(face.normal < num_normals);
    const Vec3 triangle_normal = normals[face.normal];
    const float light_intensity = -1.0f * vec3_dot(renderer->light_direction, triangle_normal);
    Color triangle_color = color_apply_intensity(color_from_u32(face.color), 0.01f);
    if (light_intensity > 0.01f) {
      triangle_color = color_apply_intensity(color_from_u32(face.color), light_intensity);
    }

    const Triangle triangle = {
        .a = vertices[face.a],
        .b = vertices[face.b],
        .c = vertices[face.c],
        .a_uv = uvs[face.a_uv],
        .b_uv = uvs[face.b_uv],
        .c_uv = uvs[face.c_uv],
        .color = color_to_u32(triangle_color),
    };

    if (renderer->cull_mode == CULL_MODE_BACKFACE) {
      const Vec3 camera_direction = vec3_sub(renderer->camera_position, triangle.a);
      const float dot_product = vec3_dot(camera_direction, triangle_normal);

      if (dot_product >= 0) {
        dyn_array_push_back(renderer->renderable_triangles, triangle);
      }
    } else {
      dyn_array_push_back(renderer->renderable_triangles, triangle);
    }
  }

  const size_t renderable_count = dyn_array_length(renderer->renderable_triangles);

  insertion_sort(renderer->renderable_triangles, renderable_count,
    sizeof(Triangle), farther_triangle, swap_render_triangles);

  for (size_t i = 0; i < renderable_count; i++) {
    const Triangle triangle = renderer->renderable_triangles[i];

    Vec4 a = mat4_mul_vec4(renderer->projection_matrix, vec3_xyzw(triangle.a));
    Vec4 b = mat4_mul_vec4(renderer->projection_matrix, vec3_xyzw(triangle.b));
    Vec4 c = mat4_mul_vec4(renderer->projection_matrix, vec3_xyzw(triangle.c));

    a = perspective_divide(a);
    b = perspective_divide(b);
    c = perspective_divide(c);

    // Scale and translate from NDC space to viewport
    a.x *= renderer->view_half_width;
    a.y *= -renderer->view_half_height;
    b.x *= renderer->view_half_width;
    b.y *= -renderer->view_half_height;
    c.x *= renderer->view_half_width;
    c.y *= -renderer->view_half_height;

    a.x += renderer->view_half_width;
    a.y += renderer->view_half_height;
    b.x += renderer->view_half_width;
    b.y += renderer->view_half_height;
    c.x += renderer->view_half_width;
    c.y += renderer->view_half_height;

    if (renderer->draw_mode & DRAW_MODE_TEXTURE && renderer->current_texture != NULL) {
      draw_textured_triangle(renderer->display, vec4_xy(a), vec4_xy(b), vec4_xy(c),
        triangle.a_uv, triangle.b_uv, triangle.c_uv, renderer->current_texture);
    }

    if (renderer->draw_mode & DRAW_MODE_TRIANGLE_FILL && !(renderer->draw_mode & DRAW_MODE_TEXTURE)) {
      draw_triangle(renderer->display, a.x, a.y, b.x, b.y, c.x, c.y, triangle.color);
    }

    if (renderer->draw_mode & DRAW_MODE_TRIANGLE_WIRE) {
      draw_line_dda(renderer->display, a.x, a.y, b.x, b.y, 0xFFFFFFFF);
      draw_line_dda(renderer->display, b.x, b.y, c.x, c.y, 0xFFFFFFFF);
      draw_line_dda(renderer->display, c.x, c.y, a.x, a.y, 0xFFFFFFFF);
    }

    if (renderer->draw_mode & DRAW_MODE_POINTS) {
      draw_rect(renderer->display, a.x, a.y, 6, 6, 0xFFFF0000);
      draw_rect(renderer->display, b.x, b.y, 6, 6, 0xFF00FF00);
      draw_rect(renderer->display, c.x, c.y, 6, 6, 0xFFFF00FF);
    }
  }
}

void renderer_end_triangles(Renderer* renderer) {
  ASSERT(renderer != NULL);
  dyn_array_clear(renderer->renderable_triangles);
}
