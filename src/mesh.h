// mesh.h
#pragma once

#include "vec.h"

#include <stdbool.h>
#include <stdint.h>

typedef struct TriangleFace {
  int a, b, c;
  int normal;
  uint32_t color;
} TriangleFace;

typedef struct Mesh {
  Vec3* vertices;
  Vec3* normals;
  TriangleFace* triangles;
} Mesh;

Mesh* init_mesh();
void destroy_mesh(Mesh* mesh);

size_t get_mesh_vertex_count(const Mesh* mesh);
size_t get_mesh_normal_count(const Mesh* mesh);
size_t get_mesh_triangle_count(const Mesh* mesh);

bool load_cube_mesh(Mesh* mesh);
bool load_obj_mesh(Mesh* mesh, const char* obj_file_path);
