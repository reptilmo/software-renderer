// mesh.h
#pragma once

#include "vec.h"

typedef struct Triangle {
  int a;
  int b;
  int c;
} Triangle;

#define NUM_MESH_VERTICES 8
#define NUM_MESH_TRIANGLES 12

extern Vec3 mesh_vertices[NUM_MESH_VERTICES];
extern Triangle mesh_triangles[NUM_MESH_TRIANGLES];
