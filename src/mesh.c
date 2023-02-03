// mesh.c
#include "mesh.h"

Vec3 mesh_vertices[NUM_MESH_VERTICES] = {
  { .x = -1, .y = -1, .z = -1 },
  { .x = -1, .y = 1,  .z = -1 },
  { .x = 1,  .y = 1,  .z = -1 },
  { .x = 1,  .y = -1, .z = -1 },
  { .x = 1,  .y = 1,  .z = 1  },
  { .x = 1,  .y = -1, .z = 1  },
  { .x = -1, .y = 1,  .z = 1  },
  { .x = -1, .y = -1, .z = 1  },
};

Triangle mesh_triangles[NUM_MESH_TRIANGLES] = {
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
