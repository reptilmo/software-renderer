// texture_mapper.c
#include "texture_mapper.h"
#include "color.h"

typedef struct Gradients {
  float one_over_w[3];
  float u_over_w[3];
  float v_over_w[3];
  float one_over_w_dx;
  float one_over_w_dy;
  float du_over_w_dx;
  float du_over_w_dy;
  float dv_over_w_dx;
  float dv_over_w_dy;
} Gradients;

typedef struct Edge {
  int y, height;
  float real_x, dx_over_dy;
  float one_over_w, one_over_w_step;
  float u_over_w, u_over_w_step;
  float v_over_w, v_over_w_step;
} Edge;

static inline void init_gradients(Gradients* g, const Triangle* triangle) {
  for (int i = 0; i < 3; i++) {
    g->one_over_w[i] = 1.0f / triangle->points[i].w;
    g->u_over_w[i] = triangle->uvs[i].x * g->one_over_w[i];
    g->v_over_w[i] = triangle->uvs[i].y * g->one_over_w[i];
  }

  const float one_over_dx = 1.0f / ((triangle->points[1].x - triangle->points[2].x) *
                                        (triangle->points[0].y - triangle->points[2].y) -
                                    (triangle->points[0].x - triangle->points[2].x) *
                                        (triangle->points[1].y - triangle->points[2].y));

  const float one_over_dy = -one_over_dx;

  g->one_over_w_dx = one_over_dx * ((g->one_over_w[1] - g->one_over_w[2]) * (triangle->points[0].y - triangle->points[2].y) -
                                    (g->one_over_w[0] - g->one_over_w[2]) * (triangle->points[1].y - triangle->points[2].y));

  g->one_over_w_dy = one_over_dy * ((g->one_over_w[1] - g->one_over_w[2]) * (triangle->points[0].x - triangle->points[2].x) -
                                    (g->one_over_w[0] - g->one_over_w[2]) * (triangle->points[1].x - triangle->points[2].x));

  g->du_over_w_dx = one_over_dx * ((g->u_over_w[1] - g->u_over_w[2]) * (triangle->points[0].y - triangle->points[2].y) -
                                   (g->u_over_w[0] - g->u_over_w[2]) * (triangle->points[1].y - triangle->points[2].y));

  g->du_over_w_dy = one_over_dy * ((g->u_over_w[1] - g->u_over_w[2]) * (triangle->points[0].x - triangle->points[2].x) -
                                   (g->u_over_w[0] - g->u_over_w[2]) * (triangle->points[1].x - triangle->points[2].x));

  g->dv_over_w_dx = one_over_dx * ((g->v_over_w[1] - g->v_over_w[2]) * (triangle->points[0].y - triangle->points[2].y) -
                                   (g->v_over_w[0] - g->v_over_w[2]) * (triangle->points[1].y - triangle->points[2].y));

  g->dv_over_w_dy = one_over_dy * ((g->v_over_w[1] - g->v_over_w[2]) * (triangle->points[0].x - triangle->points[2].x) -
                                   (g->v_over_w[0] - g->v_over_w[2]) * (triangle->points[1].x - triangle->points[2].x));
}

static inline void init_edge(Edge* edge, const Gradients* g, const Triangle* triangle, int top, int bottom) {
  edge->y = (int)ceilf(triangle->points[top].y);
  edge->height = (int)ceilf(triangle->points[bottom].y) - edge->y;

  const float y_prestep = edge->y - triangle->points[top].y;
  const float dx = triangle->points[bottom].x - triangle->points[top].x;
  const float dy = triangle->points[bottom].y - triangle->points[top].y;

  edge->real_x = y_prestep * dx / dy + triangle->points[top].x;
  edge->dx_over_dy = dx / dy;

  const float x_prestep = edge->real_x - triangle->points[top].x;

  edge->one_over_w = g->one_over_w[top] + y_prestep * g->one_over_w_dy + x_prestep * g->one_over_w_dx;
  edge->u_over_w = g->u_over_w[top] + y_prestep * g->du_over_w_dy + x_prestep * g->du_over_w_dx;
  edge->v_over_w = g->v_over_w[top] + y_prestep * g->dv_over_w_dy + x_prestep * g->dv_over_w_dx;

  edge->one_over_w_step = g->one_over_w_dy + edge->dx_over_dy * g->one_over_w_dx;
  edge->u_over_w_step = g->du_over_w_dy + edge->dx_over_dy * g->du_over_w_dx;
  edge->v_over_w_step = g->dv_over_w_dy + edge->dx_over_dy * g->dv_over_w_dx;
}

static inline void edge_step(Edge* edge) {
  edge->y++;
  edge->real_x += edge->dx_over_dy;
  edge->one_over_w += edge->one_over_w_step;
  edge->u_over_w += edge->u_over_w_step;
  edge->v_over_w += edge->v_over_w_step;
}

static inline void draw_scan_line(Display* display, const Gradients* gradients, const Edge* left, const Edge* right, const Texture* texture, float intensity) {
  const int x_start = (int)ceilf(left->real_x);
  const int width = (int)ceilf(right->real_x) - x_start;
  const float x_prestep = x_start - left->real_x;

  uint32_t* pixel_bits = display->pixel_buffer + display->width * left->y + x_start;
  float* depth_bits = display->depth_buffer + display->width * left->y + x_start;
  uint32_t* texture_bits = texture->bitmap;

  const int scale_width = texture->width - 1;
  const int scale_height = texture->height - 1;

  float one_over_w = left->one_over_w + x_prestep * gradients->one_over_w_dx;
  float u_over_w = left->u_over_w + x_prestep * gradients->du_over_w_dx;
  float v_over_w = left->v_over_w + x_prestep * gradients->dv_over_w_dx;

  for (int x = 0; x < width; x++) {
    const float w = 1.0f / one_over_w;

    const float u = u_over_w * w;
    const float v = v_over_w * w;

    int iu = (int)(u * scale_width + 0.5f);
    int iv = (int)(v * scale_height + 0.5f);

    // FIXME: Not sure what's going on with my fill convention here.
    if (iu < 0) {
      iu = 0;
    }

    if (iu > scale_width) {
      iu = scale_width;
    }

    if (iv < 0) {
      iv = 0;
    }

    if (iv > scale_height) {
      iv = scale_height;
    }

    if (one_over_w > *(depth_bits + x)) {
      *(pixel_bits + x) = color_apply_intensity(*(texture_bits + (texture->width * iv) + iu), intensity);
      *(depth_bits + x) = one_over_w;
    }

    one_over_w += gradients->one_over_w_dx;
    u_over_w += gradients->du_over_w_dx;
    v_over_w += gradients->dv_over_w_dx;
  }
}

void draw_textured_triangle(Display* display, const Triangle* triangle, const Texture* texture) {
  int8_t min, mid, max;

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
    draw_scan_line(display, &gradients, left, right, texture, triangle->light_intensity);
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
    draw_scan_line(display, &gradients, left, right, texture, triangle->light_intensity);
    edge_step(left);
    edge_step(right);
  }
}
