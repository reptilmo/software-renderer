// config.c
#include "config.h"

void process_command_line(Config* config, int argc, char* argv[]) {
  ASSERT(config != NULL);
  memset(config, 0, sizeof(Config));

  int requested_width = 0;
  int requested_height = 0;

  for (int i = 1; i < argc; i++) {
    if (strcmp(argv[i], "--fullscreen") == 0) {
      config->full_screen = true;
    } else if (sscanf(argv[i], "--w%d", &requested_width) == 1) {
      continue;
    } else if (sscanf(argv[i], "--h%d", &requested_height) == 1) {
      continue;
    } else if (strncmp(argv[i], "--mesh=", 7) == 0) {
      config->mesh_file = argv[i] + 7;
    } else if (strncmp(argv[i], "--texture=", 10) == 0) {
      config->texture_file = argv[i] + 10;
    }
  }

  if (requested_width > 0 && requested_height > 0) {
    config->window_width = requested_width;
    config->window_height = requested_height;
  } else {
    config->window_width = 800;
    config->window_height = 600;
  }
}
