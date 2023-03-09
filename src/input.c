// input_handler.c
#include "input.h"

void init_input_handler(InputHandler* input_handler, const ConfigMap* config_map) {
  memset(input_handler->keyboard_keys, 0, sizeof(bool) * 512);
  memset(input_handler->mouse_buttons, 0, sizeof(bool) * 24);

  input_handler->key_move_forward = config_map_value_int(config_map, "move_forward", 82);
  input_handler->key_move_back = config_map_value_int(config_map, "move_back", 81);
  input_handler->key_move_left = config_map_value_int(config_map, "move_left", 80);
  input_handler->key_move_right = config_map_value_int(config_map, "move_right", 79);
  input_handler->key_move_up = config_map_value_int(config_map, "move_up", 98);
  input_handler->key_move_down = config_map_value_int(config_map, "move_down", 228);
  input_handler->mouse_primary = config_map_value_int(config_map, "mouse_primary", 0);
  input_handler->mouse_secondary = config_map_value_int(config_map, "mouse_secondary", 1);
  input_handler->mouse_tertiary = config_map_value_int(config_map, "mouse_tertiary", 2);
  input_handler->mouse_sensitivity = config_map_value_float(config_map, "mouse_sensitivity", 0.5f);

  SDL_SetRelativeMouseMode(SDL_TRUE);
}

bool input_handler_update(InputHandler* input_handler) {
  int key_count = 0;
  const uint8_t* key_state = SDL_GetKeyboardState(&key_count);

  if (key_state[SDL_SCANCODE_ESCAPE]) {
    return true;
  }

  for (int k = 0; k < key_count; ++k) {
    input_handler->keyboard_keys[k] = (int)key_state[k];
  }

  const uint32_t button_state = SDL_GetRelativeMouseState(&input_handler->delta_x, &input_handler->delta_y);

  if (button_state & SDL_BUTTON(1)) {
    input_handler->mouse_buttons[0] = true;
  } else {
    input_handler->mouse_buttons[0] = false;
  }

  if (button_state & SDL_BUTTON(2)) {
    input_handler->mouse_buttons[1] = true;
  } else {
    input_handler->mouse_buttons[1] = false;
  }

  if (button_state & SDL_BUTTON(3)) {
    input_handler->mouse_buttons[2] = true;
  } else {
    input_handler->mouse_buttons[2] = false;
  }

  return false;
}
