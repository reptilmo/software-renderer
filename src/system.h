// system.h
#pragma once

#include <float.h>
#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>

#include <SDL.h>

#define RADIANS(degrees) ((degrees) * ((float)M_PI / 180.0f))

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

inline void swap_int(int* a, int* b) {
  int tmp = *a;
  *a = *b;
  *b = tmp;
}

inline void swap_float(float* a, float* b) {
  float tmp = *a;
  *a = *b;
  *b = tmp;
}

inline bool almost_equal(float a, float b, float tolerance) {
  const float diff = fabsf(a - b);

  a = fabsf(a);
  b = fabsf(b);

  const float largest = b > a ? b : a;

  if (diff <= largest * tolerance) {
    return true;
  }

  return false;
}
