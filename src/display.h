// display.h
#pragma once

#include <stdbool.h>
#include <stdint.h>

#include <SDL.h>

typedef struct Display {
	bool fullscreen;
	int width;
	int	height;
	SDL_Window* window;
	SDL_Renderer* renderer;
	SDL_Texture* pixel_buffer_texture;
	uint32_t* pixel_buffer;
} Display;

bool initialize_display(Display* display, int width, int height, bool fullscreen);
void finalize_display(Display* display);

void clear_pixel_buffer(Display* display, uint32_t color);
void present_pixel_buffer(Display* display);

void draw_grid(Display* display, int step, uint32_t color);
void draw_rect(Display* display, int left, int top, int width, int height, uint32_t color);





