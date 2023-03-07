// config.c
#include "config.h"

static inline void remove_white_space(char* str) {
  char* str2 = str;
  do {
    while (isspace(*str2)) {
      ++str2;
    }
  } while (*str++ = *str2++);
}

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

ConfigMap* init_config_map(const char* config_file_path) {
  ConfigMap* config_map = (ConfigMap*)malloc(sizeof(ConfigMap));
  if (config_map == NULL) {
    return NULL;
  }

  memset(config_map, 0, sizeof(ConfigMap));

  FILE* config_file = fopen(config_file_path, "r");
  if (config_file == NULL) {
    fprintf(stderr, "Failed to open %s!\n", config_file_path);
    return config_map;
  }

  char line[256];
  size_t key_count = 0;

  while (true) {
    memset(line, 0, sizeof(char) * 256);
    if (fgets(line, 255, config_file) == NULL) {
      break;
    }

    remove_white_space(line);
    if (sscanf(line, "%16s=%63s", &config_map->keys[key_count], &config_map->values[key_count]) >= 1) {
      key_count++;
    }
  }

  config_map->key_count = key_count;
  fclose(config_file);
  return config_map;
}

void destroy_config_map(ConfigMap* config_map) {
  if (config_map != NULL) {
    free(config_map);
  }
}

const char* config_map_value_str(const ConfigMap* config_map, const char* key) {
  const size_t len = min(strlen(key), 16);
	for (size_t i = 0; i < config_map->key_count; i++) {
    if (strncmp(config_map->keys[i], key, len)) {
      return config_map->values[i];
    }
	}

  return NULL;
}

bool config_map_value_bool(const ConfigMap* config_map, const char* key) {
  if (config_map_value_str(config_map, key) != NULL) {
    return true;
  }

  return false;
}

int config_map_value_int(const ConfigMap* config_map, const char* key) {
  const char* value = config_map_value_str(config_map, key);
  if (value != NULL) {
    return atoi(value);
  }

  return 0;
}

float config_map_value_float(const ConfigMap* config_map, const char* key) {
  const char* value = config_map_value_str(config_map, key);
  if (value != NULL) {
    return strtof(value, NULL);
  }

  return 0.0f;
}

