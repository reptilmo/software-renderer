// renderer.c
#include "renderer.h"
#include "color.h"
#include "darray.h"
#include "clipping.h"

Renderer* init_renderer(Display* display) {
  ASSERT(display != NULL);
  if (display != NULL) {
    Renderer* renderer = (Renderer*)malloc(sizeof(Renderer));
    if (renderer != NULL) {
      const float fov_over_two = 60.0f * 0.5f;
      const float near_plane = 1.0f;
      const float far_plane = 1000.0f;

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
      renderer->light_mode = LIGHT_MODE_NONE;
      renderer->view_frustum = frustum_make_view_frustum(fov_over_two, near_plane, far_plane);
      renderer->projection_matrix = mat4_make_perspective(
          (float)renderer->display->width,
          (float)renderer->display->height,
          fov_over_two, near_plane, far_plane);

      renderer->view_half_width = display->width * 0.5f;
      renderer->view_half_height = display->height * 0.5f;

      return renderer;
    }
  }

  return NULL;
}

void destroy_renderer(Renderer* renderer) {
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

void renderer_light_mode(Renderer* renderer, LightMode light_mode) {
  ASSERT(renderer != NULL);
  if (renderer != NULL) {
    renderer->light_mode = light_mode;
  }
}

void renderer_camera_position(Renderer* renderer, const Vec3* position) {
  ASSERT(renderer != NULL);
  if (renderer != NULL) {
    renderer->camera_position = *position;
  }
}

void renderer_clear_color(Renderer* renderer, uint32_t color) {
  ASSERT(renderer != NULL);
  if (renderer != NULL) {
    renderer->clear_color = color;
  }
}

void renderer_light_direction(Renderer* renderer, const Vec3* light_direction) {
  ASSERT(renderer != NULL);
  if (renderer != NULL) {
    renderer->light_direction = *light_direction;
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
  //draw_grid(renderer->display, 10, 0xFF555555);
}

void renderer_end_frame(Renderer* renderer) {
  ASSERT(renderer != NULL);
  present_pixel_buffer(renderer->display);
  //present_depth_buffer(renderer->display);
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

    if (renderer->cull_mode == CULL_MODE_BACK_FACE) {
      const Vec3 camera_direction = vec3_sub(&renderer->camera_position, &triangle_vertex_a);
      if (vec3_dot(&camera_direction, &triangle_normal) < 0) {
        continue;
      }
    }

    float light_intensity = 1.0f;
    if (renderer->light_mode > LIGHT_MODE_NONE) {
      //light_intensity = -1.0f * vec3_dot(&renderer->light_direction, &triangle_normal);
    }

    Polygon triangle;
    triangle.vertex_count = 3;
    triangle.vertices[0] = vec3_xyzw(&vertices[face.a]);
    triangle.vertices[1] = vec3_xyzw(&vertices[face.b]);
    triangle.vertices[2] = vec3_xyzw(&vertices[face.c]);
    triangle.uvs[0] = uvs[face.a_uv];
    triangle.uvs[1] = uvs[face.b_uv];
    triangle.uvs[2] = uvs[face.c_uv];
    triangle.light_intensity = light_intensity;

    dyn_array_push_back(renderer->renderable_triangles, triangle);
  }

  const size_t renderable_count = dyn_array_length(renderer->renderable_triangles);
  for (size_t i = 0; i < renderable_count; i++) {
    Polygon* polygon = &renderer->renderable_triangles[i];
    ASSERT(polygon->vertex_count == 3);
    polygon->vertices[0] = mat4_mul_vec4(renderer->projection_matrix, polygon->vertices[0]);
    polygon->vertices[1] = mat4_mul_vec4(renderer->projection_matrix, polygon->vertices[1]);
    polygon->vertices[2] = mat4_mul_vec4(renderer->projection_matrix, polygon->vertices[2]);

    axis_side_clip_polygon(W_AXIS, 1.0f, polygon);
    axis_side_clip_polygon(W_AXIS, -1.0f, polygon);
    axis_side_clip_polygon(X_AXIS, 1.0f, polygon);
    axis_side_clip_polygon(X_AXIS, -1.0f, polygon);
    axis_side_clip_polygon(Y_AXIS, 1.0f, polygon);
    axis_side_clip_polygon(Y_AXIS, -1.0f, polygon);
    axis_side_clip_polygon(Z_AXIS, 1.0f, polygon);
    axis_side_clip_polygon(Z_AXIS, -1.0f, polygon);

    for (int vertex_idx = 0; vertex_idx < polygon->vertex_count; vertex_idx++) {
      polygon->vertices[vertex_idx] = perspective_divide(&polygon->vertices[vertex_idx]);
      // Scale and translate from NDC space to screen space.
      polygon->vertices[vertex_idx].x *= renderer->view_half_width;
      polygon->vertices[vertex_idx].y *= -renderer->view_half_height;
      polygon->vertices[vertex_idx].x += renderer->view_half_width - 0.5f;
      polygon->vertices[vertex_idx].y += renderer->view_half_height - 0.5f;
    }

    const Vec4* p0 = &polygon->vertices[0];
    const Vec2* uv0 = &polygon->uvs[0];
    for (int idx = 1; idx <= polygon->vertex_count - 2; idx++) {
      Triangle triangle;
      triangle.points[0] = *p0;
      triangle.points[1] = polygon->vertices[idx];
      triangle.points[2] = polygon->vertices[idx + 1];
      triangle.uvs[0] = *uv0;
      triangle.uvs[1] = polygon->uvs[idx];
      triangle.uvs[2] = polygon->uvs[idx + 1];
      triangle.light_intensity = polygon->light_intensity;

      if (renderer->draw_mode & DRAW_MODE_TEXTURE && renderer->current_texture != NULL) {
        draw_triangle_textured(renderer->display, &triangle, renderer->current_texture);
      }

      if (renderer->draw_mode & DRAW_MODE_TRIANGLE_FILL && !(renderer->draw_mode & DRAW_MODE_TEXTURE)) {
        draw_triangle_shaded(renderer->display, &triangle);
      }

      //FIXME:
      const Vec4* a = &triangle.points[0];
      const Vec4* b = &triangle.points[1];
      const Vec4* c = &triangle.points[2];

      if (renderer->draw_mode & DRAW_MODE_TRIANGLE_WIRE) {
        draw_line_dda(renderer->display, (int)a->x, (int)a->y, (int)b->x, (int)b->y, 0xFFFF0000);
        draw_line_dda(renderer->display, (int)b->x, (int)b->y, (int)c->x, (int)c->y, 0xFFFF0000);
        draw_line_dda(renderer->display, (int)c->x, (int)c->y, (int)a->x, (int)a->y, 0xFFFF0000);
      }

      if (renderer->draw_mode & DRAW_MODE_POINTS) {
        draw_rect(renderer->display, (int)a->x, (int)a->y, 6, 6, 0xFFFF0000);
        draw_rect(renderer->display, (int)b->x, (int)b->y, 6, 6, 0xFF00FF00);
        draw_rect(renderer->display, (int)c->x, (int)c->y, 6, 6, 0xFF0000FF);
      }
    }
  }
}

void renderer_end_triangles(Renderer* renderer) {
  ASSERT(renderer != NULL);
  dyn_array_clear(renderer->renderable_triangles);
}
