// view_frustum.c
#include "view_frustum.h"
#include "system.h"

Frustum frustum_make_view_frustum(float fov_over_two, float near, float far) {
	const float cos_f = cosf(RADIANS(fov_over_two));
	const float sin_f = sinf(RADIANS(fov_over_two));

	Frustum frustum;
	frustum.planes[FRUSTUM_NEAR_PLANE].point = vec3_new(0.0f, 0.0f, near);
	frustum.planes[FRUSTUM_NEAR_PLANE].normal = vec3_new(0.0f, 0.0f, 1.0f);

	frustum.planes[FRUSTUM_FAR_PLANE].point = vec3_new(0.0f, 0.0f, far);
	frustum.planes[FRUSTUM_FAR_PLANE].normal = vec3_new(0.0f, 0.0f, -1.0f);

	frustum.planes[FRUSTUM_LEFT_PLANE].point = vec3_new(0.0f, 0.0f, 0.0f);
	frustum.planes[FRUSTUM_LEFT_PLANE].normal = vec3_new(cos_f, 0.0f, sin_f);

	frustum.planes[FRUSTUM_RIGHT_PLANE].point = vec3_new(0.0f, 0.0f, 0.0f);
	frustum.planes[FRUSTUM_RIGHT_PLANE].normal = vec3_new(-cos_f, 0.0f, sin_f);

	frustum.planes[FRUSTUM_TOP_PLANE].point = vec3_new(0.0f, 0.0f, 0.0f);
	frustum.planes[FRUSTUM_TOP_PLANE].normal = vec3_new(0.0f, -cos_f, sin_f);

	frustum.planes[FRUSTUM_BOTTOM_PLANE].point = vec3_new(0.0f, 0.0f, 0.0f);
	frustum.planes[FRUSTUM_BOTTOM_PLANE].normal = vec3_new(0.0f, cos_f, sin_f);
	return frustum;
}

static inline void plane_clip_polygon(const Plane* plane, Polygon* polygon) {
	// http://www.idav.ucdavis.edu/education/GraphicsNotes/Clipping/Clipping.html
	const Vec3 P = plane->point;
	const Vec3 N = plane->normal;
	const int vertex_count = polygon->vertex_count;

	Vec3 in[POLYGON_MAX_VERTICES];
	int in_count = 0;

	Vec3 PQ = vec3_sub(&polygon->vertices[0], &P);
	float pdot = 0.0f;
	float idot = vec3_dot(&N, &PQ);

	for (int i = 1; i < vertex_count; i++) {
		PQ = vec3_sub(&polygon->vertices[i], &P);
		const float dot = vec3_dot(&N, &PQ);

		if ((dot * pdot) < 0.0f) {
			const float t = pdot / (pdot - dot);
			in[in_count++] = vec3_lerp(&polygon->vertices[i - 1], &polygon->vertices[i], t);
		}

		if (dot > 0.0f) {
			in[in_count++] = polygon->vertices[i];
			pdot = dot;
		}
	}

	if ((pdot * idot) < 0) {
		const float t = pdot / (pdot - idot);
		in[in_count++] = vec3_lerp(&polygon->vertices[vertex_count - 1], &polygon->vertices[0], t);
	}

	for (int i = 0; i < in_count; i++) {
		polygon->vertices[i] = in[i];
		polygon->vertex_count = in_count;
	}
}

void frustum_clip_polygon(const Frustum* frustum, Polygon* polygon) {
	plane_clip_polygon(&frustum->planes[FRUSTUM_NEAR_PLANE], polygon);
	plane_clip_polygon(&frustum->planes[FRUSTUM_FAR_PLANE], polygon);
	plane_clip_polygon(&frustum->planes[FRUSTUM_LEFT_PLANE], polygon);
	plane_clip_polygon(&frustum->planes[FRUSTUM_RIGHT_PLANE], polygon);
	plane_clip_polygon(&frustum->planes[FRUSTUM_TOP_PLANE], polygon);
	plane_clip_polygon(&frustum->planes[FRUSTUM_BOTTOM_PLANE], polygon);
}
