// sort.c
#include "sort.h"

void insertion_sort(void* array, size_t array_length, size_t element_size,
                    bool (*less)(void* a, void* b), void (*swap)(void* a, void* b)) {

  uint8_t* current = (uint8_t*)array;
  uint8_t* end = (uint8_t*)array + (array_length * element_size);

  while (current < end) {
    uint8_t* inner = current;

    while (inner > (uint8_t*)array && less(inner, inner - element_size)) {
      swap(inner, inner - element_size);
      inner -= element_size;
    }

    current += element_size;
  }
}
