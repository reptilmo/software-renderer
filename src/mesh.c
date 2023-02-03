// mesh.c
#include "mesh.h"
#include "darray.h"

#include <assert.h>

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

size_t get_mesh_vertex_count(Mesh* mesh) {
  assert(mesh != NULL);
  if (mesh != NULL) {
    return dyn_array_length(mesh->vertices);
  }
  return 0;
}

size_t get_mesh_triangle_count(Mesh* mesh) {
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
