// input.h
#pragma once

#include "system.h"
#include "config.h"
#include "display.h"

typedef struct InputHandler {
  bool keyboard_keys[512];
  bool mouse_buttons[24];

  int kb_move_forward;
  int kb_move_back;
  int kb_move_left;
  int kb_move_right;
  int kb_move_up;
  int kb_move_down;
  int kb_look_up;
  int kb_look_down;
  int kb_look_right;
  int kb_look_left;

  int mouse_primary;
  int mouse_secondary;
  int mouse_tertiary;

  float mouse_sensitivity;
  int delta_x;
  int delta_y;
} InputHandler;

void init_input_handler(InputHandler* input_handler, const ConfigMap* config_map);
bool input_handler_update(InputHandler* input_handler, Display* display);

static inline bool move_forward(const InputHandler* input_handler) {
  return input_handler->keyboard_keys[input_handler->kb_move_forward];
}

static inline bool move_back(const InputHandler* input_handler) {
  return input_handler->keyboard_keys[input_handler->kb_move_back];
}

static inline bool move_left(const InputHandler* input_handler) {
  return input_handler->keyboard_keys[input_handler->kb_move_left];
}

static inline bool move_right(const InputHandler* input_handler) {
  return input_handler->keyboard_keys[input_handler->kb_move_right];
}

static inline bool move_up(const InputHandler* input_handler) {
  return input_handler->keyboard_keys[input_handler->kb_move_up];
}

static inline bool move_down(const InputHandler* input_handler) {
  return input_handler->keyboard_keys[input_handler->kb_move_down];
}

static inline bool look_left(const InputHandler* input_handler) {
  return input_handler->keyboard_keys[input_handler->kb_look_left];
}

static inline bool look_right(const InputHandler* input_handler) {
  return input_handler->keyboard_keys[input_handler->kb_look_right];
}

static inline bool look_up(const InputHandler* input_handler) {
  return input_handler->keyboard_keys[input_handler->kb_look_up];
}

static inline bool look_down(const InputHandler* input_handler) {
  return input_handler->keyboard_keys[input_handler->kb_look_down];
}

static inline bool mouse_primary_pressed(const InputHandler* input_handler) {
  return input_handler->mouse_buttons[input_handler->mouse_primary];
}

static inline bool mouse_secondary_pressed(const InputHandler* input_handler) {
  return input_handler->mouse_buttons[input_handler->mouse_secondary];
}

static inline bool mouse_tertiary_pressed(const InputHandler* input_handler) {
  return input_handler->mouse_buttons[input_handler->mouse_tertiary];
}

static inline float mouse_delta_x(const InputHandler* input_handler) {
  return input_handler->delta_x * input_handler->mouse_sensitivity;
}

static inline float mouse_delta_y(const InputHandler* input_handler) {
  return input_handler->delta_y * input_handler->mouse_sensitivity;
}
