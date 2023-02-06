// renderer.c
#include "renderer.h"
#include "darray.h"

#include <assert.h>

Renderer* init_renderer(Display* display) {
  assert(display != NULL);
  if (display != NULL) {
    Renderer* renderer = (Renderer*)malloc(sizeof(Renderer));
    if (renderer != NULL) {
      renderer->display = display;
      renderer->renderable_triangles = NULL;
      renderer->cull_backface = false;
      renderer->display_half_width = display->width / 2;
      renderer->display_half_height = display->height / 2;
      renderer->camera_position.x = 0.0f;
      renderer->camera_position.y = 0.0f;
      renderer->camera_position.z = 0.0f;
      renderer->clear_color = 0xFF000000;

      return renderer;
    }
  }

  return NULL;
}

void destroy_renderer(Renderer* renderer) {
  assert(renderer != NULL);
  if (renderer != NULL) {
    dyn_array_free(renderer->renderable_triangles);
    free(renderer);
  }
}

void renderer_backface_culling(Renderer* renderer, bool cull_backface) {
  assert(renderer != NULL);
  if (renderer != NULL) {
    renderer->cull_backface = cull_backface;
  }
}

void renderer_camera_position(Renderer* renderer, Vec3 position) {
  assert(renderer != NULL);
  if (renderer != NULL) {
    renderer->camera_position = position;
  }
}

void renderer_clear_color(Renderer* renderer, uint32_t color) {
  assert(renderer != NULL);
  if (renderer != NULL) {
    renderer->clear_color = color;
  }
}

void renderer_begin_frame(Renderer* renderer) {
  assert(renderer != NULL);

  clear_pixel_buffer(renderer->display, renderer->clear_color);
  draw_grid(renderer->display, 10, 0xFF333333);
}

void renderer_end_frame(Renderer* renderer) {
  assert(renderer != NULL);

  present_pixel_buffer(renderer->display);
}

void renderer_begin_triangles(Renderer* renderer, TriangleFace* faces, size_t num_faces,
  Vec3* vertices, size_t num_vertices, Vec3* normals, size_t num_normals) {

  assert(renderer != NULL);

  for (size_t face_idx = 0; face_idx < num_faces; face_idx++) {

    const TriangleFace face = faces[face_idx];
    assert(face.a < num_vertices);
    assert(face.b < num_vertices);
    assert(face.c < num_vertices);

    const Triangle triangle = {
        .a = vertices[face.a],
        .b = vertices[face.b],
        .c = vertices[face.c],
        .color = face.color,
    };
    
    if (renderer->cull_backface) {
      //const Vec3 triangle_normal = vec3_cross(vec3_sub(triangle.b, triangle.a), vec3_sub(triangle.c, triangle.a));

      assert(face.normal < num_normals);
      const Vec3 triangle_normal = normals[face.normal];
      const Vec3 camera_direction = vec3_sub(renderer->camera_position, triangle.a);

      if (vec3_dot(camera_direction, triangle_normal) >= 0) {
        dyn_array_push_back(renderer->renderable_triangles, triangle);
      }
    } else {
      dyn_array_push_back(renderer->renderable_triangles, triangle);
    }
  }

  const size_t renderable_count = dyn_array_length(renderer->renderable_triangles);

  for (size_t i = 0; i < renderable_count; i++) {
    const Triangle triangle = renderer->renderable_triangles[i];

    Vec2 a = project(triangle.a);
    Vec2 b = project(triangle.b);
    Vec2 c = project(triangle.c);

    a.x += renderer->display_half_width;
    a.y += renderer->display_half_height;
    b.x += renderer->display_half_width;
    b.y += renderer->display_half_height;
    c.x += renderer->display_half_width;
    c.y += renderer->display_half_height;

    draw_rect(renderer->display, a.x, a.y, 6, 6, 0xFFFFFF00);
    draw_rect(renderer->display, b.x, b.y, 6, 6, 0xFFFFFF00);
    draw_rect(renderer->display, c.x, c.y, 6, 6, 0xFFFFFF00);

    draw_line_dda(renderer->display, a.x, a.y, b.x, b.y, triangle.color);
    draw_line_dda(renderer->display, b.x, b.y, c.x, c.y, triangle.color);
    draw_line_dda(renderer->display, c.x, c.y, a.x, a.y, triangle.color);
  }
}

void renderer_end_triangles(Renderer *renderer) {
  assert(renderer != NULL);
  dyn_array_clear(renderer->renderable_triangles);
}
