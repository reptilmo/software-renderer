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

enum {
  CONFIG_MAP_MAX_KEYS = 64,
};

typedef struct ConfigMap {
  char keys[CONFIG_MAP_MAX_KEYS][16];
  char values[CONFIG_MAP_MAX_KEYS][96];
  size_t key_count;
} ConfigMap;

void process_command_line(Config* config, int argc, char* argv[]);

ConfigMap* init_config_map(const char* config_file);
void destroy_config_map(ConfigMap* config_map);
bool config_map_value_bool(const ConfigMap* config_map, const char* key);
int config_map_value_int(const ConfigMap* config_map, const char* key);
float config_map_value_float(const ConfigMap* config_map, const char* key);
const char* config_map_value_str(const ConfigMap* config_map, const char* key);
