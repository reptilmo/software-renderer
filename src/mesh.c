// mesh.c
#include "mesh.h"
#include "darray.h"

#include <assert.h>
#include <stdio.h>
#include <string.h>

#define NUM_CUBE_VERTICES 8
#define NUM_CUBE_TRIANGLES 12

static Vec3 cube_vertices[NUM_CUBE_VERTICES] = {
  { .x = -1, .y = -1, .z = -1 },
  { .x = -1, .y = 1,  .z = -1 },
  { .x = 1,  .y = 1,  .z = -1 },
  { .x = 1,  .y = -1, .z = -1 },
  { .x = 1,  .y = 1,  .z = 1  },
  { .x = 1,  .y = -1, .z = 1  },
  { .x = -1, .y = 1,  .z = 1  },
  { .x = -1, .y = -1, .z = 1  },
};

static Triangle cube_triangles[NUM_CUBE_TRIANGLES] = {
  { .a = 0, .b = 1, .c = 3 }, // Front
  { .a = 1, .b = 2, .c = 3 },

  { .a = 3, .b = 2, .c = 4 }, // Right
  { .a = 3, .b = 4, .c = 5 },

  { .a = 6, .b = 7, .c = 4 }, // Back
  { .a = 7, .b = 5, .c = 4 },

  { .a = 0, .b = 7, .c = 6 }, // Left
  { .a = 0, .b = 6, .c = 1 },

  { .a = 1, .b = 6, .c = 2 }, // Top
  { .a = 2, .b = 6, .c = 4 },

  { .a = 0, .b = 5, .c = 7  }, // Bottom
  { .a = 0, .b = 3, .c = 5  },
};

Mesh* init_mesh() {
  Mesh* mesh = (Mesh*)malloc(sizeof(Mesh));
  assert(mesh != NULL);
  if (mesh != NULL) {
    mesh->vertices = NULL;
    mesh->triangles = NULL;
  }
  return mesh;
}

size_t get_mesh_vertex_count(const Mesh* mesh) {
  assert(mesh != NULL);
  if (mesh != NULL) {
    return dyn_array_length(mesh->vertices);
  }
  return 0;
}

size_t get_mesh_triangle_count(const Mesh* mesh) {
  assert(mesh != NULL);
  if (mesh != NULL) {
    return dyn_array_length(mesh->triangles);
  }
  return 0;
}

void load_cube_mesh(Mesh* mesh) {
  assert(mesh != NULL);
  if (mesh != NULL) {
    for (int i = 0; i < NUM_CUBE_VERTICES; i++) {
      dyn_array_push_back(mesh->vertices, cube_vertices[i]);
    }
    for (int i = 0; i < NUM_CUBE_TRIANGLES; i++) {
      dyn_array_push_back(mesh->triangles, cube_triangles[i]);
    }
  }
}

bool load_obj_mesh(Mesh* mesh, const char* obj_file_path) {
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
        Vec3 v = {.x = f[0], .y = -f[1], .z = f[2]};
        dyn_array_push_back(mesh->vertices, v);
      }
    } else if (strncmp(buf, "f ", 2) == 0) {
      int vertex_idx[3] = {0};
      int texture_idx[3] = {0};
      int normal_idx[3] = {0};

      if (sscanf(buf, "f %i/%i/%i\t%i/%i/%i\t%i/%i/%i\n",
        &vertex_idx[0], &texture_idx[0], &normal_idx[0],
        &vertex_idx[1], &texture_idx[1], &normal_idx[1],
        &vertex_idx[2], &texture_idx[2], &normal_idx[2]) == 9) {

        Triangle tri = {
          .a = vertex_idx[0] - 1,
          .b = vertex_idx[1] - 1,
          .c = vertex_idx[2] - 1,
        };

        dyn_array_push_back(mesh->triangles, tri);
      }
    }
  }

  fclose(mesh_file);
  return true;
}

