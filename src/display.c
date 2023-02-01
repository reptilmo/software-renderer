// display.c
#include "display.h"

#include <stdio.h>
#include <assert.h>

bool initialize_display(Display* display, int width, int height, bool fullscreen) {
  assert(display != NULL);
  display->width = width;
  display->height = height;
  display->fullscreen = fullscreen;

  display->window = SDL_CreateWindow(
    "Software Renderer", //NULL,
    SDL_WINDOWPOS_CENTERED,
    SDL_WINDOWPOS_CENTERED,
    width,
    height,
    0//SDL_WINDOW_BORDERLESS
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
    height
  );

  if (display->pixel_buffer_texture == NULL) {
    fprintf(stderr, "Error in SDL create pixel buffer texture!\n");
    return false;
  }

  display->pixel_buffer = (uint32_t*) malloc(sizeof(uint32_t) * display->width * display->height);
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
    (int)display->width * sizeof(uint32_t)
  );

  SDL_RenderCopy(display->renderer, display->pixel_buffer_texture, NULL, NULL);
  SDL_RenderPresent(display->renderer);
}

void draw_grid(Display* display, int step, uint32_t color) {

  for (int y = 0; y < display->height; y += step) {
    for (int x = 0; x < display->width; x += step) {
      //if (x % 10 == 0 || y % step == 0) {
      display->pixel_buffer[display->width * y + x] = color;
      //}
    }
  }
}

void draw_rect(Display* display, int left, int top, int width, int height, uint32_t color) {
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


