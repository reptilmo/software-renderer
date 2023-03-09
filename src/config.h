// config.h
#pragma once

#include "system.h"

enum {
  CONFIG_MAP_MAX_KEYS = 64,
  CONFIG_MAP_KEY_LEN = 31,
  CONFIG_MAP_VAL_LEN = 41,
};

typedef struct ConfigMap {
  char keys[CONFIG_MAP_MAX_KEYS][CONFIG_MAP_KEY_LEN];
  char values[CONFIG_MAP_MAX_KEYS][CONFIG_MAP_VAL_LEN];
  size_t key_count;
} ConfigMap;

void init_config_map(ConfigMap* config_map);
const char* config_map_value_str(const ConfigMap* config_map, const char* key, const char* default_value);
int config_map_value_int(const ConfigMap* config_map, const char* key, int default_value);
float config_map_value_float(const ConfigMap* config_map, const char* key, float default_value);

void process_command_line(ConfigMap* config_map, int argc, char* argv[]);
