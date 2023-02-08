// darray.h
#pragma once

#include <stdlib.h>

#define dyn_array_push_back(array, element)                \
  do {                                                     \
    (array) = dyn_array_alloc(array, 1, sizeof(*(array))); \
    (array)[dyn_array_length(array) - 1] = (element);      \
  } while (0)

void* dyn_array_alloc(void* array, size_t element_count, size_t element_size);
size_t dyn_array_length(void* array);
size_t dyn_array_clear(void* array); // Returns current capacity.
void dyn_array_free(void* array);
