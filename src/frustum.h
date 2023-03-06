// frustum.h
#pragma once

#include "darray.h"
#include "triangle.h"
#include "vec.h"

typedef struct Plane {
  Vec3 point;
  Vec3 normal;
} Plane;

typedef enum FrustumPlane {
  FRUSTUM_NEAR_PLANE = 0,
  FRUSTUM_FAR_PLANE = 1,
  FRUSTUM_LEFT_PLANE = 2,
  FRUSTUM_RIGHT_PLANE = 3,
  FRUSTUM_TOP_PLANE = 4,
  FRUSTUM_BOTTOM_PLANE = 5,

  FRUSTUM_PLANE_COUNT = 6,
} FrustumPlane;

typedef struct Frustum {
  Plane planes[FRUSTUM_PLANE_COUNT];
} Frustum;

Frustum frustum_make_view_frustum(float fov_over_two, float near, float far);
//void frustum_clip_polygon(const Frustum* frustum, Polygon* polygon);
