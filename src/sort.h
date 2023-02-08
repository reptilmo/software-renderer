// sort.h
#pragma once

#include <stdbool.h>
#include <stdint.h>

void insertion_sort(void* array, size_t array_length,
                    size_t element_size, bool (*less)(void* a, void* b), void (*swap)(void* a, void* b));
