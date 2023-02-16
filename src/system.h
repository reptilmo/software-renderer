// system.h
#pragma once

#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <SDL.h>

#define INLINE inline
#define RADIANS(degrees) ((degrees)*0.01745329f)

#ifndef RELEASE
#define ASSERT(condition)                                                                              \
  do {                                                                                                 \
    if (!(condition)) {                                                                                \
      fprintf(stderr, "Assertion failed in %s:%d\nCondition: %s\n", __FILE__, __LINE__, (#condition)); \
      abort();                                                                                         \
    }                                                                                                  \
  } while (0);
#else
#define ASSERT(condition) (condition)
#endif

INLINE void swap_int(int* a, int* b) {
  int tmp = *a;
  *a = *b;
  *b = tmp;
}

INLINE void swap_float(float* a, float* b) {
  float tmp = *a;
  *a = *b;
  *b = tmp;
}

INLINE bool nearly_zero(float f) {
  ASSERT(0) // FIXME: Implement!
  return true;
}
