// display.c
#include "display.h"

#include <assert.h>
#include <stdio.h>

bool initialize_display(Display* display, int width, int height, bool fullscreen) {
  assert(display != NULL);
  display->width = width;
  display->height = height;
  display->fullscreen = fullscreen;

  display->window = SDL_CreateWindow(
      "Software Renderer", // NULL,
      SDL_WINDOWPOS_CENTERED,
      SDL_WINDOWPOS_CENTERED,
      width,
      height,
      0 // SDL_WINDOW_BORDERLESS
  );

  if (display->window == NULL) {
    fprintf(stderr, "Error in SDL create window!\n");
    return false;
  }

  display->renderer = SDL_CreateRenderer(display->window, -1, 0);

  if (display->renderer == NULL) {
    fprintf(stderr, "Error in SDL create renderer!\n");
    return false;
  }

  display->pixel_buffer_texture = SDL_CreateTexture(
      display->renderer,
      SDL_PIXELFORMAT_ARGB8888,
      SDL_TEXTUREACCESS_STREAMING,
      width,
      height);

  if (display->pixel_buffer_texture == NULL) {
    fprintf(stderr, "Error in SDL create pixel buffer texture!\n");
    return false;
  }

  display->pixel_buffer = (uint32_t*)malloc(sizeof(uint32_t) * display->width * display->height);
  if (display->pixel_buffer == NULL) {
    fprintf(stderr, "Failed to allocate pixel buffer!\n");
    return false;
  }

  return true;
}

void finalize_display(Display* display) {
  if (display->pixel_buffer != NULL) {
    free(display->pixel_buffer);
  }

  if (display->pixel_buffer_texture != NULL) {
    SDL_DestroyTexture(display->pixel_buffer_texture);
  }

  if (display->renderer != NULL) {
    SDL_DestroyRenderer(display->renderer);
  }

  if (display->window != NULL) {
    SDL_DestroyWindow(display->window);
  }
}

void clear_pixel_buffer(Display* display, uint32_t color) {
  const int len = display->width * display->height;

  for (int i = 0; i < len; i++) {
    display->pixel_buffer[i] = color;
  }
}

void present_pixel_buffer(Display* display) {
  SDL_UpdateTexture(
      display->pixel_buffer_texture,
      NULL,
      display->pixel_buffer,
      (int)display->width * sizeof(uint32_t));

  SDL_RenderCopy(display->renderer, display->pixel_buffer_texture, NULL, NULL);
  SDL_RenderPresent(display->renderer);
}

void draw_grid(Display* display, int step, uint32_t color) {

  for (int y = 0; y < display->height; y += step) {
    for (int x = 0; x < display->width; x += step) {
      // if (x % 10 == 0 || y % step == 0) {
      display->pixel_buffer[display->width * y + x] = color;
      //}
    }
  }
}

void draw_rect(Display* display, int left, int top, int width, int height, uint32_t color) {

  if (left < 0) {
    left = 0;
  }
  if (top < 0) {
    top = 0;
  }

  int right = left + width;
  if (right >= display->width) {
    right = display->width - 1;
  }

  int bottom = top + height;
  if (bottom >= display->height) {
    bottom = display->height - 1;
  }

  for (int y = top; y <= bottom; y++) {
    for (int x = left; x <= right; x++) {
      display->pixel_buffer[display->width * y + x] = color;
    }
  }
}

void draw_line_dda(Display* display, int x0, int y0, int x1, int y1, uint32_t color) {
  const int dx = x1 - x0;
  const int dy = y1 - y0;

  const int run_length = abs(dx) > abs(dy) ? abs(dx) : abs(dy);

  const float sx = dx / (float)run_length;
  const float sy = dy / (float)run_length;

  float px = (float)x0;
  float py = (float)y0;

  for (int i = 0; i <= run_length; i++) {
    display->pixel_buffer[display->width * (int)round(py) + (int)round(px)] = color; //FIXME: Clip to back buffer dimentions!
    px += sx;
    py += sy;
  }
}

inline void swap_int(int* a, int* b) {
  int tmp = *b;
  *b = *a;
  *a = tmp;
}

static void draw_scanline(Display* display, int x0, int x1, int y, uint32_t color) {
  if (y >= display->height) {
    return;
  }

  if (x0 > x1) {
    swap_int(&x0, &x1);
  }

  if (x0 < 0) {
    x0 = 0;
  }

  if (x1 >= display->width) {
    x1 = display->width - 1;
  }

  y = display->width * y;

  for (int x = x0; x <= x1; x++) {
    display->pixel_buffer[y + x] = color;
  }
}

static void draw_flat_bottom_triangle(Display* display, int x0, int y0, int x1, int y1, int x2, int y2, uint32_t color) {
  const float inv_slope_left = (x1 - x0) / (float)(y1 - y0);
  const float inv_slope_right = (x2 - x0) / (float)(y2 - y0);
  float scanline_start = (float)x0;
  float scanline_end = (float)x0;

  for (int y = y0; y <= y2; y++) {
    draw_scanline(display, (int)round(scanline_start), (int)round(scanline_end), y, color);
    scanline_start += inv_slope_left;
    scanline_end += inv_slope_right;
  }
}

static void draw_flat_top_triangle(Display* display, int x0, int y0, int x1, int y1, int x2, int y2, uint32_t color) {
  const float inv_slope_left = (x2 - x0) / (float)(y2 - y0);
  const float inv_slope_right = (x2 - x1) / (float)(y2 - y1);
  float scanline_start = (float)x2;
  float scanline_end = (float)x2;

  for (int y = y2; y >= y0; y--) {
    draw_scanline(display, (int)round(scanline_start), (int)round(scanline_end), y, color);
    scanline_start -= inv_slope_left;
    scanline_end -= inv_slope_right;
  }
}

void draw_triangle(Display* display, int x0, int y0, int x1, int y1, int x2, int y2, uint32_t color) {

  if (y0 > y1) {
    swap_int(&y0, &y1);
    swap_int(&x0, &x1);
  }

  if (y1 > y2) {
    swap_int(&y1, &y2);
    swap_int(&x1, &x2);
  }

  if (y0 > y1) {
    swap_int(&y0, &y1);
    swap_int(&x0, &x1);
  }

  if (y1 == y2) {
    draw_flat_bottom_triangle(display, x0, y0, x1, y1, x2, y2, color);
  } else if (y0 == y1) {
    draw_flat_top_triangle(display, x0, y0, x1, y1, x2, y2, color);
  } else {
    const int ym = y1;
    const int xm = (y1 - y0) * (x2 - x0) / (float)(y2 - y0) + x0;

    draw_flat_bottom_triangle(display, x0, y0, x1, y1, xm, ym, color);
    draw_flat_top_triangle(display, xm, ym, x1, y1, x2, y2, color);
  }
}


