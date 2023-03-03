// view_frustum.h
#pragma once

#include "vec.h"
#include "triangle.h"

enum {
	POLYGON_MAX_VERTICES = 9,
};

typedef struct Polygon {
	Vec3 vertices[POLYGON_MAX_VERTICES];
	int vertex_count;
} Polygon;

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


INLINE void polygon_from_triangle(Polygon* polygon, const Triangle* triangle) {
	//FIXME: Maybe define two triangle types: TransformedTriangle and RenderableTriangle.
	polygon->vertices[0] = vec4_xyz(&triangle->points[0]);
	polygon->vertices[1] = vec4_xyz(&triangle->points[1]);
	polygon->vertices[2] = vec4_xyz(&triangle->points[2]);
	polygon->vertex_count = 3;
}

Frustum frustum_make_view_frustum(float fov_over_two, float near, float far);
void frustum_clip_polygon(const Frustum* frustum, Polygon* polygon);

