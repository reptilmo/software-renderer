// mesh.c
#include "mesh.h"
#include "darray.h"

#define NUM_CUBE_VERTICES 8
#define NUM_CUBE_UVS 4
#define NUM_CUBE_NORMALS 6
#define NUM_CUBE_TRIANGLES 12

static Vec3 cube_vertices[NUM_CUBE_VERTICES] = {
    {.x = -1, .y = -1, .z = -1}, // 0
    {.x = -1, .y = 1, .z = -1},  // 1
    {.x = 1, .y = 1, .z = -1},   // 2
    {.x = 1, .y = -1, .z = -1},  // 3
    {.x = 1, .y = 1, .z = 1},    // 4
    {.x = 1, .y = -1, .z = 1},   // 5
    {.x = -1, .y = 1, .z = 1},   // 6
    {.x = -1, .y = -1, .z = 1},  // 7
};

static Vec2 cube_uvs[NUM_CUBE_UVS] = {
    {.x = 1.0f, .y = 1.0f},
    {.x = 0.0f, .y = 1.0f},
    {.x = 0.0f, .y = 0.0f},
    {.x = 1.0f, .y = 0.0f},
};

static Vec3 cube_normals[NUM_CUBE_NORMALS] = {
    {.x = 0, .y = 0, .z = -1}, // 0 Front
    {.x = 1, .y = 0, .z = 0},  // 1 Right
    {.x = 0, .y = 0, .z = 1},  // 2 Back
    {.x = -1, .y = 0, .z = 0}, // 3 Left
    {.x = 0, .y = 1, .z = 0},  // 4 Top
    {.x = 0, .y = -1, .z = 0}, // 5 Bottom
};

static TriangleFace cube_triangles[NUM_CUBE_TRIANGLES] = {
    {.a = 1, .b = 3, .c = 0, .a_uv = 1, .b_uv = 3, .normal = 0}, // Front
    {.a = 3, .b = 1, .c = 2, .a_uv = 3, .b_uv = 1, .normal = 0},
    {.a = 3, .b = 2, .c = 4, .a_uv = 2, .b_uv = 1, .normal = 1}, // Right
    {.a = 4, .b = 5, .c = 3, .a_uv = 2, .b_uv = 3, .normal = 1},
    {.a = 6, .b = 7, .c = 5, .a_uv = 2, .b_uv = 1, .normal = 2}, // Back
    {.a = 5, .b = 6, .c = 4, .a_uv = 0, .b_uv = 2, .normal = 2},
    {.a = 1, .b = 7, .c = 0, .a_uv = 0, .b_uv = 2, .normal = 3}, // Left
    {.a = 6, .b = 7, .c = 1, .a_uv = 1, .b_uv = 2, .normal = 3},
    {.a = 6, .b = 2, .c = 1, .a_uv = 1, .b_uv = 3, .normal = 4}, // Top
    {.a = 2, .b = 6, .c = 4, .a_uv = 3, .b_uv = 1, .normal = 4},
    {.a = 0, .b = 5, .c = 7, .a_uv = 1, .b_uv = 3, .normal = 5}, // Bottom
    {.a = 0, .b = 3, .c = 5, .a_uv = 1, .b_uv = 0, .normal = 5},
};

Mesh* init_mesh() {
  Mesh* mesh = (Mesh*)malloc(sizeof(Mesh));
  if (mesh != NULL) {
    mesh->vertices = NULL;
    mesh->normals = NULL;
    mesh->triangles = NULL;
    mesh->uvs = NULL;
  }
  return mesh;
}

void destroy_mesh(Mesh* mesh) {
  if (mesh != NULL) {
    dyn_array_free(mesh->vertices);
    dyn_array_free(mesh->normals);
    dyn_array_free(mesh->uvs);
    dyn_array_free(mesh->triangles);

    free(mesh);
  }
}

size_t mesh_vertex_count(const Mesh* mesh) {
  ASSERT(mesh != NULL);
  if (mesh != NULL) {
    return dyn_array_length(mesh->vertices);
  }
  return 0;
}

size_t mesh_normal_count(const Mesh* mesh) {
  ASSERT(mesh != NULL);
  if (mesh != NULL) {
    return dyn_array_length(mesh->normals);
  }
  return 0;
}

size_t mesh_triangle_count(const Mesh* mesh) {
  ASSERT(mesh != NULL);
  if (mesh != NULL) {
    return dyn_array_length(mesh->triangles);
  }
  return 0;
}

size_t mesh_uv_count(const Mesh* mesh) {
  ASSERT(mesh != NULL);
  if (mesh != NULL) {
    return dyn_array_length(mesh->uvs);
  }
  return 0;
}

bool mesh_load_cube(Mesh* mesh) {
  ASSERT(mesh != NULL);
  if (mesh == NULL) {
    return false;
  }

  for (int i = 0; i < NUM_CUBE_VERTICES; i++) {
    dyn_array_push_back(mesh->vertices, cube_vertices[i]);
  }
  for (int i = 0; i < NUM_CUBE_NORMALS; i++) {
    dyn_array_push_back(mesh->normals, cube_normals[i]);
  }
  for (int i = 0; i < NUM_CUBE_UVS; i++) {
    dyn_array_push_back(mesh->uvs, cube_uvs[i]);
  }
  for (int i = 0; i < NUM_CUBE_TRIANGLES; i++) {
    dyn_array_push_back(mesh->triangles, cube_triangles[i]);
  }

  return true;
}

bool mesh_load_obj(Mesh* mesh, const char* obj_file_path) {
  ASSERT(mesh != NULL);
  if (mesh == NULL) {
    return false;
  }

  FILE* mesh_file = fopen(obj_file_path, "r");
  if (mesh_file == NULL) {
    fprintf(stderr, "Failed to open %s!\n", obj_file_path);
    return false;
  }

  char buf[256];

  while (true) {
    memset(buf, 0, sizeof(char) * 256);
    if (fgets(buf, 256, mesh_file) == NULL) {
      break;
    }

    if (strncmp(buf, "v ", 2) == 0) {
      float f[3] = {0.0f};

      if (sscanf(buf, "v %f %f %f\n", &f[0], &f[1], &f[2]) == 3) {
        Vec3 vertex = {.x = f[0], .y = f[1], .z = f[2]};
        dyn_array_push_back(mesh->vertices, vertex);
      }
    } else if (strncmp(buf, "vn ", 3) == 0) {
      float f[3] = {0.0f};

      if (sscanf(buf, "vn %f %f %f\n", &f[0], &f[1], &f[2]) == 3) {
        Vec3 normal = {.x = f[0], .y = f[1], .z = f[2]};
        dyn_array_push_back(mesh->normals, normal);
      }
    } else if (strncmp(buf, "vt ", 3) == 0) {
      float f[2] = {0.0f};

      if (sscanf(buf, "vt %f %f\n", &f[0], &f[1]) == 2) {
        Vec2 uv = {.x = f[0], .y = 1.0f - f[1]};
        dyn_array_push_back(mesh->uvs, uv);
      }
    } else if (strncmp(buf, "f ", 2) == 0) {
      int vertex_idx[3] = {0};
      int uv_idx[3] = {0};
      int normal_idx[3] = {0};

      if (sscanf(buf, "f %i/%i/%i\t%i/%i/%i\t%i/%i/%i\n",
                 &vertex_idx[0], &uv_idx[0], &normal_idx[0],
                 &vertex_idx[1], &uv_idx[1], &normal_idx[1],
                 &vertex_idx[2], &uv_idx[2], &normal_idx[2]) == 9) {

        TriangleFace tri = {
            .a = vertex_idx[0] - 1,
            .b = vertex_idx[1] - 1,
            .c = vertex_idx[2] - 1,
            .a_uv = uv_idx[0] - 1,
            .b_uv = uv_idx[1] - 1,
            .c_uv = uv_idx[2] - 1,
            .normal = normal_idx[0] - 1,
        };

        dyn_array_push_back(mesh->triangles, tri);
      }
    }
  }

  fclose(mesh_file);
  return true;
}
