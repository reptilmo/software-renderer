// main.c
#include <stdio.h>
#include <stdbool.h>

#include <SDL.h>

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;

bool create_sdl_window(void);
void setup(void);
void process_input(bool* );
void update(void);
void render(void);


int main(int argc, char *argv[]) {

  bool is_running = create_sdl_window();

  setup();

  while (is_running) {
    process_input(&is_running);
    update();
    render();
  }

  return 0;
}

bool create_sdl_window(void) {
  if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
    fprintf(stderr, "Error in SDL init.\n");
    return false;
  }

  window = SDL_CreateWindow(
    NULL,
    SDL_WINDOWPOS_CENTERED,
    SDL_WINDOWPOS_CENTERED,
    800,
    600,
    SDL_WINDOW_BORDERLESS
  );

  if (window == NULL) {
    fprintf(stderr, "Error in SDL create window.\n");
    return false;
  }

  renderer = SDL_CreateRenderer(window, -1, 0);

  if (renderer == NULL) {
    fprintf(stderr, "Error in SDL create renderer.\n");
    return false;

  }

  return true;
}

void setup(void) {
}

void process_input(bool* is_running) {
  SDL_Event event;
  SDL_PollEvent(&event);

  switch (event.type) {
  case SDL_QUIT:
    is_running = false;
    break;
  case SDL_KEYDOWN:
    if (event.key.keysym.scancode == SDL_SCANCODE_ESCAPE) {
      *is_running = false;
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

  SDL_RenderPresent(renderer);
}

