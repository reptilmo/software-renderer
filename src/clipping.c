// clipping.c
#include "clipping.h"

void axis_side_clip_polygon(const Axis axis, float side, Polygon* polygon) {
  const int vertex_count = polygon->vertex_count;

  Vec4 clipped_vertices[POLYGON_MAX_VERTICES];
  Vec2 clipped_uvs[POLYGON_MAX_VERTICES];
  int clipped_count = 0;

  Vec2* prev_uv = &polygon->uvs[vertex_count - 1];
  Vec4* previous = &polygon->vertices[vertex_count - 1];
  int8_t previous_dot = (side * vec4_axis(previous, axis)) <= vec4_axis(previous, W_AXIS) ? 1 : -1;

  for (int i = 0; i < vertex_count; i++) {
    Vec2* cur_uv = &polygon->uvs[i];
    Vec4* current = &polygon->vertices[i];
    int8_t dot = (side * vec4_axis(current, axis)) <= vec4_axis(current, W_AXIS) ? 1 : -1;

    if ((previous_dot * dot) < 0) {
      // (W-p - P-axis) / (W-p - P-axis) - (W-c - C-axis)
      const float factor = (vec4_axis(previous, W_AXIS) - vec4_axis(previous, axis) * side) /
        ((vec4_axis(previous, W_AXIS) - vec4_axis(previous, axis) * side) -
          (vec4_axis(current, W_AXIS) - vec4_axis(current, axis) * side));

      // N = P + f(C - P);
      clipped_vertices[clipped_count] = vec4_lerp(previous, current, factor);
      clipped_uvs[clipped_count] = vec2_lerp(prev_uv, cur_uv, factor);
      clipped_count++;
    }

    if (dot > 0) {
      clipped_vertices[clipped_count] = *current;
      clipped_uvs[clipped_count] = *cur_uv;
      clipped_count++;
    }

    previous_dot = dot;
    previous = current;
    prev_uv = cur_uv;
  }

  polygon->vertex_count = clipped_count;
  for (int i = 0; i < clipped_count; i++) {
    polygon->vertices[i] = clipped_vertices[i];
    polygon->uvs[i] = clipped_uvs[i];
  }
}
