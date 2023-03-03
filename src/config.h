// config.h
#pragma once

#include "system.h"

typedef struct Config {
  bool full_screen;
  int window_width;
  int window_height;
  const char* mesh_file;
  const char* texture_file;
} Config;

void process_command_line(Config* config, int argc, char* argv[]);
