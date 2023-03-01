// triangle_shaded.c
#include "color.h"
#include "triangle.h"

typedef struct Gradients {
  float one_over_w[3];
  float one_over_w_dx, one_over_w_dy;
} Gradients;

typedef struct Edge {
  int y, height;
  float real_x, dx_over_dy;
  float one_over_w, one_over_w_step;
} Edge;

static inline void init_gradients(Gradients* g, const Triangle* triangle) {
  for (int i = 0; i < 3; i++) {
    g->one_over_w[i] = 1.0f / triangle->points[i].w;
  }

  const float one_over_dx = 1.0f / ((triangle->points[1].x - triangle->points[2].x) * (triangle->points[0].y - triangle->points[2].y) -
                                    (triangle->points[0].x - triangle->points[2].x) * (triangle->points[1].y - triangle->points[2].y));

  const float one_over_dy = -one_over_dx;

  g->one_over_w_dx = one_over_dx * ((g->one_over_w[1] - g->one_over_w[2]) * (triangle->points[0].y - triangle->points[2].y) -
                                    (g->one_over_w[0] - g->one_over_w[2]) * (triangle->points[1].y - triangle->points[2].y));

  g->one_over_w_dy = one_over_dy * ((g->one_over_w[1] - g->one_over_w[2]) * (triangle->points[0].x - triangle->points[2].x) -
                                    (g->one_over_w[0] - g->one_over_w[2]) * (triangle->points[1].x - triangle->points[2].x));
}

static inline void init_edge(Edge* edge, const Gradients* g, const Triangle* triangle, int top, int bottom) {
  edge->y = (int)ceilf(triangle->points[top].y);
  edge->height = (int)ceilf(triangle->points[bottom].y) - edge->y;

  const float dx = triangle->points[bottom].x - triangle->points[top].x;
  const float dy = triangle->points[bottom].y - triangle->points[top].y;
  const float y_prestep = edge->y - triangle->points[top].y;

  edge->dx_over_dy = dx / dy;
  edge->real_x = y_prestep * edge->dx_over_dy + triangle->points[top].x;

  const float x_prestep = edge->real_x - triangle->points[top].x;

  edge->one_over_w = g->one_over_w[top] + y_prestep * g->one_over_w_dy + x_prestep * g->one_over_w_dx;
  edge->one_over_w_step = g->one_over_w_dy + edge->dx_over_dy * g->one_over_w_dx;
}

static inline void edge_step(Edge* edge) {
  edge->y++;
  edge->real_x += edge->dx_over_dy;
  edge->one_over_w += edge->one_over_w_step;
}

static inline void draw_scan_line(Display* display, const Gradients* g, const Edge* left, const Edge* right, uint32_t color, float intensity) {
  const int x_start = (int)ceilf(left->real_x);
  const int width = (int)ceilf(right->real_x) - x_start;
  const float x_prestep = x_start - left->real_x;

  uint32_t* pixel_bits = display->pixel_buffer + display->width * left->y + x_start;
  float* depth_bits = display->depth_buffer + display->width * left->y + x_start;

  float one_over_w = left->one_over_w + x_prestep * g->one_over_w_dx;

  for (int x = 0; x < width; x++) {
    if (one_over_w > *(depth_bits + x)) {
      *(pixel_bits + x) = color_apply_intensity(color, intensity);
      *(depth_bits + x) = one_over_w;
    }

    one_over_w += g->one_over_w_dx;
  }
}

void draw_triangle_shaded(Display* display, const Triangle* triangle) {
  int min, mid, max;

  const float y0 = triangle->points[0].y;
  const float y1 = triangle->points[1].y;
  const float y2 = triangle->points[2].y;

  if (y0 < y1) {
    if (y2 < y0) {
      min = 2;
      mid = 0;
      max = 1;
    } else {
      min = 0;
      if (y1 < y2) {
        mid = 1;
        max = 2;
      } else {
        mid = 2;
        max = 1;
      }
    }
  } else {
    if (y2 < y1) {
      min = 2;
      mid = 1;
      max = 0;
    } else {
      min = 1;
      if (y0 < y2) {
        mid = 0;
        max = 2;
      } else {
        mid = 2;
        max = 0;
      }
    }
  }

  int mid_is_left = ((triangle->points[mid].x - triangle->points[min].x) *
                         (triangle->points[max].y - triangle->points[min].y) -
                     (triangle->points[max].x - triangle->points[min].x) *
                         (triangle->points[mid].y - triangle->points[min].y)) < 0;

  Gradients gradients;
  init_gradients(&gradients, triangle);
  Edge top_to_middle;
  init_edge(&top_to_middle, &gradients, triangle, min, mid);
  Edge top_to_bottom;
  init_edge(&top_to_bottom, &gradients, triangle, min, max);

  Edge* left = NULL;
  Edge* right = NULL;

  if (mid_is_left) {
    left = &top_to_middle;
    right = &top_to_bottom;
  } else {
    left = &top_to_bottom;
    right = &top_to_middle;
  }

  int height = top_to_middle.height;

  while (height--) {
    draw_scan_line(display, &gradients, left, right, triangle->color, triangle->light_intensity);
    edge_step(left);
    edge_step(right);
  }

  Edge middle_to_bottom;
  init_edge(&middle_to_bottom, &gradients, triangle, mid, max);

  if (mid_is_left) {
    left = &middle_to_bottom;
    right = &top_to_bottom;
  } else {
    left = &top_to_bottom;
    right = &middle_to_bottom;
  }

  height = middle_to_bottom.height;

  while (height--) {
    draw_scan_line(display, &gradients, left, right, triangle->color, triangle->light_intensity);
    edge_step(left);
    edge_step(right);
  }
}
