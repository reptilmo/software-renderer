// sort.h
#pragma once

#include "system.h"

void insertion_sort(void* array, size_t element_count,
                    size_t element_size, bool (*less)(void* a, void* b), void (*swap)(void* a, void* b));
