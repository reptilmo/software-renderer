// mesh.h
#pragma once

#include "system.h"
#include "vec.h"

typedef struct TriangleFace {
  int a, b, c;
  int a_uv, b_uv, c_uv;
  int normal;
} TriangleFace;

typedef struct Mesh {
  Vec3* vertices;
  Vec3* normals;
  Vec2* uvs;
  TriangleFace* triangles;
} Mesh;

Mesh* init_mesh(void);
void destroy_mesh(Mesh* mesh);

size_t mesh_vertex_count(const Mesh* mesh);
size_t mesh_normal_count(const Mesh* mesh);
size_t mesh_triangle_count(const Mesh* mesh);
size_t mesh_uv_count(const Mesh* mesh);

bool mesh_load_cube(Mesh* mesh);
bool mesh_load_obj(Mesh* mesh, const char* obj_file_path);
