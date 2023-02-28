// renderer.c
#include "renderer.h"
#include "color.h"
#include "darray.h"
#include "sort.h"
#include "texture_mapper.h"

void swap_render_triangles(void* left, void* right) {
  Triangle tmp = *(Triangle*)left;
  *(Triangle*)left = *(Triangle*)right;
  *(Triangle*)right = tmp;
}

bool farther_triangle(void* left, void* right) {

  const float left_avg_z = (((Triangle*)left)->points[0].z +
                            ((Triangle*)left)->points[1].z +
                            ((Triangle*)left)->points[2].z) /
                           3.0f;

  const float right_avg_z = (((Triangle*)right)->points[0].z +
                             ((Triangle*)right)->points[1].z +
                             ((Triangle*)right)->points[2].z) /
                            3.0f;

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

void renderer_light_direction(Renderer* renderer, Vec3 light_direction) {
  ASSERT(renderer != NULL);
  if (renderer != NULL) {
    renderer->light_direction = light_direction;
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
  clear_depth_buffer(renderer->display, 0.0f);
  clear_pixel_buffer(renderer->display, renderer->clear_color);
  // draw_grid(renderer->display, 10, 0xFF333333);
}

void renderer_end_frame(Renderer* renderer) {
  ASSERT(renderer != NULL);
  present_pixel_buffer(renderer->display);
  // present_depth_buffer(renderer->display);
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

    ASSERT(face.normal < num_normals);
    const Vec3 triangle_normal = normals[face.normal];
    const Vec3 triangle_vertex_a = vertices[face.a];

    if (renderer->cull_mode == CULL_MODE_BACKFACE) {
      const Vec3 camera_direction = vec3_sub(&renderer->camera_position, &triangle_vertex_a);
      if (vec3_dot(&camera_direction, &triangle_normal) < 0) {
        continue;
      }
    }

    const float light_intensity = -1.0f * vec3_dot(&renderer->light_direction, &triangle_normal);

    Triangle triangle;
    triangle.points[0] = vec3_xyzw(&vertices[face.a]);
    triangle.points[1] = vec3_xyzw(&vertices[face.b]);
    triangle.points[2] = vec3_xyzw(&vertices[face.c]);
    triangle.uvs[0] = uvs[face.a_uv];
    triangle.uvs[1] = uvs[face.b_uv];
    triangle.uvs[2] = uvs[face.c_uv];
    triangle.light_intensity = light_intensity;

    dyn_array_push_back(renderer->renderable_triangles, triangle);
  }

  const size_t renderable_count = dyn_array_length(renderer->renderable_triangles);

  /*insertion_sort(renderer->renderable_triangles, renderable_count,
                 sizeof(Triangle), farther_triangle, swap_render_triangles);*/

  for (size_t i = 0; i < renderable_count; i++) {
    Triangle* triangle = &renderer->renderable_triangles[i];

    Vec4 a = mat4_mul_vec4(renderer->projection_matrix, triangle->points[0]);
    Vec4 b = mat4_mul_vec4(renderer->projection_matrix, triangle->points[1]);
    Vec4 c = mat4_mul_vec4(renderer->projection_matrix, triangle->points[2]);

    a = perspective_divide(&a);
    b = perspective_divide(&b);
    c = perspective_divide(&c);

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

    triangle->points[0] = a;
    triangle->points[1] = b;
    triangle->points[2] = c;

    if (renderer->draw_mode & DRAW_MODE_TEXTURE && renderer->current_texture != NULL) {
      draw_textured_triangle(renderer->display, triangle, renderer->current_texture);
    }

    if (renderer->draw_mode & DRAW_MODE_TRIANGLE_FILL && !(renderer->draw_mode & DRAW_MODE_TEXTURE)) {
      draw_triangle(renderer->display, a.x, a.y, b.x, b.y, c.x, c.y, color_apply_intensity(0xFFFFFFFF, triangle->light_intensity));
    }

    if (renderer->draw_mode & DRAW_MODE_TRIANGLE_WIRE) {
      draw_line_dda(renderer->display, a.x, a.y, b.x, b.y, 0xFFFFFFFF);
      draw_line_dda(renderer->display, b.x, b.y, c.x, c.y, 0xFFFFFFFF);
      draw_line_dda(renderer->display, c.x, c.y, a.x, a.y, 0xFFFFFFFF);
    }

    if (renderer->draw_mode & DRAW_MODE_POINTS) {
      draw_rect(renderer->display, a.x, a.y, 6, 6, 0xFFFFFF00);
      draw_rect(renderer->display, b.x, b.y, 6, 6, 0xFFFFFF00);
      draw_rect(renderer->display, c.x, c.y, 6, 6, 0xFFFFFF00);
    }
  }
}

void renderer_end_triangles(Renderer* renderer) {
  ASSERT(renderer != NULL);
  dyn_array_clear(renderer->renderable_triangles);
}
