// darray.c
#include "darray.h"

#define RAW_ARRAY(array) (((size_t*)(array)) - 2)
#define ARRAY_CAPACITY(array) (RAW_ARRAY(array)[0])
#define ARRAY_LENGTH(array) (RAW_ARRAY(array)[1])

void* dyn_array_alloc(void* array, size_t element_count, size_t element_size) {

  if (array == NULL) {
    size_t raw_size = sizeof(size_t) * 2 + element_count * element_size;
    size_t* raw_array = (size_t*)malloc(raw_size);
    ASSERT(raw_array != NULL);
    raw_array[0] = element_count;
    raw_array[1] = element_count;
    return raw_array + 2;
  } else if (ARRAY_LENGTH(array) + element_count <= ARRAY_CAPACITY(array)) {
    ARRAY_LENGTH(array) += element_count;
    return array;
  } else {
    size_t needed_capacity = ARRAY_LENGTH(array) + element_count;
    size_t new_capacity = needed_capacity > ARRAY_CAPACITY(array) * 2 ? needed_capacity : ARRAY_CAPACITY(array) * 2;
    size_t raw_size = sizeof(size_t) * 2 + new_capacity * element_size;
    size_t* raw_array = (size_t*)realloc(RAW_ARRAY(array), raw_size);
    ASSERT(raw_array != NULL);
    raw_array[0] = new_capacity;
    raw_array[1] = needed_capacity;
    return raw_array + 2;
  }
}

size_t dyn_array_length(void* array) {
  return array != NULL ? ARRAY_LENGTH(array) : 0;
}

size_t dyn_array_clear(void* array) {
  if (array != NULL) {
    ARRAY_LENGTH(array) = 0;
    return ARRAY_CAPACITY(array);
  }
  return 0;
}

void dyn_array_free(void* array) {
  if (array != NULL) {
    free(RAW_ARRAY(array));
  }
}
