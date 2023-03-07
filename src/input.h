// input.h
#pragma once

#include "system.h"

typedef struct Input {
	bool keyboard_keys[512];
	bool mouse_buttons[24];

	int key_move_forward;
	int key_move_back;
	int key_move_left;
	int key_move_right;

	int mouse_primary;
	int mouse_secondary;
	int mouse_tertiary;

	float mouse_sensitivity;
	float delta_x;
	float delat_y;
	
} Input;

