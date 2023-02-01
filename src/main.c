// main.c
#include <stdio.h>

#include "display.h"

bool setup(void);
void process_input(bool* running);
void update(void);
void render(void);

Display display = {0};

int main(int argc, char *argv[]) {

  if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) != 0) {
    fprintf(stderr, "Error from SDL init!\n");
    return 1;
  }

  if (!initialize_display(&display, 800, 600, false)) {
    return 1;
  }

  bool running = setup();

  while (running) {
    process_input(&running);
    update();
    render();
  }

  finalize_display(&display);
  SDL_Quit();

  return 0;
}

bool setup(void) {
  return true;
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
  //TODO:
}

void render(void) {
  clear_pixel_buffer(&display, 0xFF000000);
  draw_grid(&display, 10, 0xFF333333);

  draw_rect(&display, 50, 50, 20, 30, 0xFFFF0000);
  draw_rect(&display, 300, 200, 70, 40, 0xFFFFFF00);

  present_pixel_buffer(&display);
}
