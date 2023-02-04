// mesh.h
#pragma once

#include "vec.h"

#include <stdbool.h>

typedef struct Triangle {
  int a;
  int b;
  int c;
} Triangle;

typedef struct ProjectedTriangle {
  Vec2 a;
  Vec2 b;
  Vec2 c;
} ProjectedTriangle;

typedef struct Mesh {
  Vec3* vertices;
  Triangle* triangles;
} Mesh;

Mesh* init_mesh();
size_t get_mesh_vertex_count(const Mesh* mesh);
size_t get_mesh_triangle_count(const Mesh* mesh);

void load_cube_mesh(Mesh* mesh);
bool load_obj_mesh(Mesh* mesh, const char* obj_file_path);
