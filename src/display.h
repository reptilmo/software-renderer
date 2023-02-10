// display.h
#pragma once

#include "system.h"

typedef struct Display {
  SDL_Window* window;
  SDL_Renderer* renderer;
  SDL_Texture* pixel_buffer_texture;
  uint32_t* pixel_buffer;
  int width;
  int height;
  bool fullscreen;
} Display;

Display* init_display(int width, int height, bool fullscreen);
void destroy_display(Display* display);

void clear_pixel_buffer(Display* display, uint32_t color);
void present_pixel_buffer(Display* display);

void draw_grid(Display* display, int step, uint32_t color);
void draw_rect(Display* display, int left, int top, int width, int height, uint32_t color);
void draw_line_dda(Display* display, int x0, int y0, int x1, int y1, uint32_t color);
void draw_triangle(Display* display, int x0, int y0, int x1, int y1, int x2, int y2, uint32_t color);
