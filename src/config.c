// config.c
#include "config.h"

static const char* CONFIG_FILE_NAME = "renderer.conf";

static inline void remove_white_space(char* str) {
  char* str2 = str;
  do {
    while (isspace(*str2)) {
      ++str2;
    }
  } while (*str++ = *str2++);
}

void init_config_map(ConfigMap* config_map) {
  ASSERT(config_map != NULL);
  memset(config_map, 0, sizeof(ConfigMap));

  char config_file_path[1000];
  const char* base_path = SDL_GetBasePath();
  const size_t base_path_len = SDL_utf8strlcpy(config_file_path, base_path, 1000);
  if (base_path_len + strlen(CONFIG_FILE_NAME) >= 1000) {
    fprintf(stderr, "Config file path too long!\n");
    return;
  }

  const size_t len = SDL_strlcat(config_file_path, CONFIG_FILE_NAME, base_path_len + strlen(CONFIG_FILE_NAME) + 1);
  config_file_path[len] = '\0';

  FILE* config_file = fopen(config_file_path, "r");
  if (config_file == NULL) {
    fprintf(stderr, "Failed to open %s!\n", config_file_path);
    return;
  }

  char line[256];
  size_t key_count = 0;

  while (true) {
    memset(line, 0, sizeof(char) * 256);
    if (fgets(line, 255, config_file) == NULL) {
      break;
    }

    if (line[0] == '#' || isspace(line[0])) {
      continue;
    }

    remove_white_space(line);
    const char* key = strtok(line, "=");
    if (strlen(key) > CONFIG_MAP_KEY_LEN) {
      fprintf(stderr, "Skipping key [%s], too long!\n", key);
      continue;
    }

    const char* value = strtok(NULL, "=");
    if (strlen(value) > CONFIG_MAP_VAL_LEN) {
      fprintf(stderr, "Config value [%s], too long!\n", value);
      continue;
    }

    memcpy(config_map->keys[key_count],  key, strlen(key));
    memcpy(config_map->values[key_count], value, strlen(value));
    key_count++;
  }

  fclose(config_file);
  config_map->key_count = key_count;
}

void destroy_config_map(ConfigMap* config_map) {
  if (config_map != NULL) {
    free(config_map);
  }
}

const char* config_map_value_str(const ConfigMap* config_map, const char* key, const char* default_value) {
  ASSERT(config_map != NULL);
  const size_t len = min(strlen(key), CONFIG_MAP_KEY_LEN);
	for (size_t i = 0; i < config_map->key_count; i++) {
    if (strncmp(config_map->keys[i], key, len) == 0) {
      return config_map->values[i];
    }
  }

  return default_value;
}

int config_map_value_int(const ConfigMap* config_map, const char* key, int default_value) {
  ASSERT(config_map != NULL);
  const char* value = config_map_value_str(config_map, key, NULL);
  if (value != NULL) {
    return atoi(value);
  }

  return default_value;
}

float config_map_value_float(const ConfigMap* config_map, const char* key, float default_value) {
  ASSERT(config_map != NULL);
  const char* value = config_map_value_str(config_map, key, NULL);
  if (value != NULL) {
    return strtof(value, NULL);
  }

  return default_value;
}

static void config_map_set(ConfigMap* config_map, const char* key, const char* value) {
  if (key == NULL || value == NULL) {
    return;
  }

  if (strlen(key) > CONFIG_MAP_KEY_LEN) {
    fprintf(stderr, "Skipping key [%s], too long!\n", key);
    return;
  }

  if (strlen(value) > CONFIG_MAP_VAL_LEN) {
    fprintf(stderr, "Config value [%s], too long!\n", value);
    return;
  }

  for (size_t i = 0; i < config_map->key_count; i++) {
    if (strncmp(config_map->keys[i], key, strlen(key)) == 0) {
      memset(config_map->values[i], 0, CONFIG_MAP_VAL_LEN);
      memcpy(config_map->values[i], value, strlen(value));
      return;
    }
  }

  if (config_map->key_count < CONFIG_MAP_MAX_KEYS) {
    strncpy(config_map->keys[config_map->key_count], key, strlen(key));
    strncpy(config_map->values[config_map->key_count], value, strlen(value));
    config_map->key_count++;
  }
}

void process_command_line(ConfigMap* config_map, int argc, char* argv[]) {
  ASSERT(config_map != NULL);

  for (int i = 1; i < argc; i++) {
    if (strcmp(argv[i], "--fullscreen") == 0) {
      config_map_set(config_map, "fullscreen", "true");
      continue;
    }

    const char* param_key = strtok(argv[i], "=");

    if (strncmp(param_key, "--w", 3) == 0) {
      const char* param_value = strtok(NULL, "=");
      config_map_set(config_map, "window_width", param_value);
    } else if (strncmp(param_key, "--h", 3) == 0) {
      const char* param_value = strtok(NULL, "=");
      config_map_set(config_map, "window_height", param_value);
    } else if (strncmp(argv[i], "--mesh", 6) == 0) {
      const char* param_value = strtok(NULL, "=");
      config_map_set(config_map, "load_mesh", param_value);
    } else if (strncmp(argv[i], "--texture", 9) == 0) {
      const char* param_value = strtok(NULL, "=");
      config_map_set(config_map, "load_texture", param_value);
    }
  }
}

