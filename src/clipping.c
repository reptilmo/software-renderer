// clipping.c
#include "clipping.h"

void axis_side_clip_polygon(const Axis axis, float side, Polygon* polygon) {
  const int vertex_count = polygon->vertex_count;

  Vec4 in[POLYGON_MAX_VERTICES];
  int in_count = 0;

  Vec4* previous = &polygon->vertices[vertex_count - 1];
  int8_t previous_dot = (side * vec4_axis(previous, axis)) <= vec4_axis(previous, W_AXIS) ? 1 : -1;

  for (int i = 0; i < vertex_count; i++) {
    Vec4* current = &polygon->vertices[i];
    int8_t dot = (side * vec4_axis(current, axis)) <= vec4_axis(current, W_AXIS) ? 1 : -1;

    if ((previous_dot * dot) < 0) {
      const float factor = (vec4_axis(previous, W_AXIS) - vec4_axis(previous, axis) * side) / ((vec4_axis(previous, W_AXIS) - vec4_axis(previous, axis) * side) - (vec4_axis(current, W_AXIS) - vec4_axis(current, axis) * side));

      // N = P + f(C - P);
      Vec4 new = vec4_lerp(previous, current, factor);
      in[in_count++] = new;
    }

    if (dot > 0) {
      in[in_count++] = *current;
    }

    previous_dot = dot;
    previous = current;
  }

  polygon->vertex_count = in_count;
  for (int i = 0; i < in_count; i++) {
    polygon->vertices[i] = in[i];
  }
}
