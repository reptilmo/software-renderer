// main.c
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include <SDL.h>

bool full_screen = false;
int window_width = 800;
int window_height = 600;

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;
SDL_Texture* pixel_buffer_texture = NULL;
uint32_t* pixel_buffer = NULL;

bool setup(void);
void teardown(void);
void process_input(bool* running);
void update(void);
void render(void);

bool create_sdl_window(int width, int height);
uint32_t* create_pixel_buffer(int width, int height);
void clear_pixel_buffer(uint32_t color);
void render_pixel_buffer(void);
void draw_grid(uint32_t color, int step);
void draw_rect(int x, int y, int width, int height, uint32_t color);


int main(int argc, char *argv[]) {

  bool running = setup();

  while (running) {
    process_input(&running);
    update();
    render();
  }

  teardown();

  return 0;
}

bool setup(void) {
  bool rval = create_sdl_window(window_width, window_height);
  if (rval) {
    pixel_buffer = create_pixel_buffer(window_width, window_height);
    if (pixel_buffer == NULL) {
      fprintf(stderr, "Error allocating pixel buffer!\n");
      rval = false;
    }
  }

  return rval;
}

void teardown(void) {
  if (pixel_buffer != NULL) {
    free(pixel_buffer);
  }

  SDL_DestroyTexture(pixel_buffer_texture);
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
}

void process_input(bool* running) {
  SDL_Event event;
  SDL_PollEvent(&event);

  switch (event.type) {
  case SDL_QUIT:
    *running = false;
    break;
  case SDL_KEYDOWN:
    if (event.key.keysym.scancode == SDL_SCANCODE_ESCAPE) {
      *running = false;
    }
    break;
  default:
    break;
  }
}

void update(void) {

}

void render(void) {
  SDL_SetRenderDrawColor(renderer, 0, 60, 200, 255);
  SDL_RenderClear(renderer);

  //TODO:
  clear_pixel_buffer(0xFF000000);
  draw_grid(0xFF333333, 10);

  draw_rect(50, 50, 20, 30, 0xFFFF0000);
  draw_rect(300, 200, 70, 40, 0xFFFFFF00);

  render_pixel_buffer();

  SDL_RenderPresent(renderer);
}

bool create_sdl_window(int width, int height) {
  if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
    fprintf(stderr, "Error in SDL init!\n");
    return false;
  }

  window = SDL_CreateWindow(
    "Software Renderer", //NULL,
    SDL_WINDOWPOS_CENTERED,
    SDL_WINDOWPOS_CENTERED,
    width,
    height,
    0//SDL_WINDOW_BORDERLESS
  );

  if (window == NULL) {
    fprintf(stderr, "Error in SDL create window!\n");
    return false;
  }

  renderer = SDL_CreateRenderer(window, -1, 0);

  if (renderer == NULL) {
    fprintf(stderr, "Error in SDL create renderer!\n");
    return false;

  }

  pixel_buffer_texture = SDL_CreateTexture(
    renderer,
    SDL_PIXELFORMAT_ARGB8888,
    SDL_TEXTUREACCESS_STREAMING,
    width,
    height
  );

  if (pixel_buffer_texture == NULL) {
    fprintf(stderr, "Error in SDL create pixel buffer texture!\n");
    return false;
  }

  return true;
}

uint32_t* create_pixel_buffer(int width, int height) {
  return (uint32_t*) malloc(sizeof(uint32_t) * width * height);

}

void clear_pixel_buffer(uint32_t color) {
  for (int y = 0; y < window_height; y++) {
    for (int x = 0; x < window_width; x++) {
      pixel_buffer[window_width * y + x] =  color;
    }
  }
}

void render_pixel_buffer(void) {
  SDL_UpdateTexture(
    pixel_buffer_texture,
    NULL,
    pixel_buffer,
    (int) window_width * sizeof(uint32_t)
  );

  SDL_RenderCopy(renderer, pixel_buffer_texture, NULL, NULL);
}

void draw_grid(uint32_t color, int step) {

  for (int y = 0; y < window_height; y += step) {
    for (int x = 0; x < window_width; x += step) {
      //if (x % 10 == 0 || y % step == 0) {
        pixel_buffer[window_width * y + x] = color;
      //}
    }
  }
}

void draw_rect(int left, int top, int width, int height, uint32_t color) {

  for (int y = top; y <= top + height && y < window_height; y++) {
    for (int x = left; x <= left + width && x < window_width; x++) {
      pixel_buffer[window_width * y + x] = color;
    }
  }

}

