/*
 * Copyright (c) 1993-1995 Argonaut Technologies Limited. All rights reserved.
 *
 * $Id: rend.h 1.1 1997/07/11 16:15:45 jon Exp JON $
 * $Locker: JON $
 *
 * Static scratch area for renderer
 */
#ifndef _REND_H_
#define _REND_H_

#ifdef __cplusplus
extern "C" {
#endif

/*
 * Flags for vertices
 */
#define TVDIR_FRONT	0x80000000
#define TVDIR_BACK	0x40000000

#define TV_CLIPPED	0x20000000

struct temp_face {

	/*
	 * Combined outcodes of face
	 */
	br_uint_16 codes;

	/*
	 * Flag describing visiblity of face
	 */
	br_uint_8 flag;
	br_uint_8 _pad[1];
};

/*
 * Values for temp_face.flag
 */
#define TFF_VISIBLE			4
#define TFF_CLIPPED			2
#define TFF_REVERSED		1

/*
 * Workspace for renderer functions
 */
typedef struct rend_block {
	/*
	 * Pointers into scratch buffer
	 */
	void *scratch;

	br_boolean model_onscreen;
	
	struct temp_face *temp_faces;
	br_int_8 *vertex_counts;

	LPD3DTLVERTEX d3dtlvertices;
	LPD3DHVERTEX d3dhvertices;
	LPD3DVALUE d3dhw;

	union brp_vertex **vertex_heap_pointers;

	br_int_8 *vertex_flags;
	br_int_8 *edge_flags;

	br_int_32 nvisible_faces;
	br_boolean faces_clipped;

	struct v11face *faces;
	struct fmt_vertex *vertices;

    br_colour *face_colours;
    br_colour *vertex_colours;

	br_uint_8 *face_flags;

	br_int_32 nfaces;
	br_int_32 nvertices;
	br_int_32 nedges;

	br_vector3 eye_l;

	struct brp_block *block;

	br_boolean block_changed;
	br_boolean range_changed;

	/*
	 * Current renderer
	 */
	struct br_renderer *renderer;
	struct br_geometry *geometry;

	/*
	 * Current subdivion limit
	 */
	br_scalar subdivide_threshold;
} _rend;

/*
 * Cached info that is based on current state
 */
struct active_light {

	br_token type;
	br_vector3	position;
	br_vector3	direction;
	br_vector3	half;
	br_scalar	intensity;
	void (*accumulate_index)(br_renderer *self, br_vector3 *p, br_vector3 *n, struct active_light *alp, br_scalar *comp);
	void (*accumulate_colour)(br_renderer *self, br_vector3 *p, br_vector3 *n, br_colour colour, struct active_light *alp, br_scalar *comp);
	struct state_light *s;
};

typedef struct static_cache {

	br_matrix4 model_to_screen;
	br_matrix34 view_to_model;
	br_matrix34 model_to_environment;

	/*
	 * model_to_screen with viewport transform multiplied in
	 */
	br_matrix4 model_to_viewport;

#if BASED_FIXED
	/*
	 * High words of bottom row of matrix
	 */
	br_scalar model_to_viewport_hi[4];
#endif

	/*
	 * Values used for scaling screen vertices
	 */
	float sx_offset;
	float sx_scale;
	float sy_offset;
	float sy_scale;

	/*
	 * Combined surface colour & opacity
	 */
	br_colour colour;

#if BASED_FIXED
	br_uint_32	loop_count;
#endif

	/*
	 * Local copy of bounds
	 */
	br_vector2	min;
	br_vector2	max;

	br_vector4 eye_m;
	br_vector3 eye_m_normalised;

	br_boolean	user_clip_active;

	/*
	 * Current lighting info.
	 */
	br_boolean light_1md;
	struct active_light lights[MAX_STATE_LIGHTS];
	br_int_32 nlights_model;
	br_int_32 nlights_view;

	/*
	 * Flags indicating what needs to be updated
	 */
	br_boolean	valid_v2m;
	br_boolean	valid_m2s;
	br_boolean	valid_per_model;
	br_boolean	valid_per_scene;

} _static_cache;

extern _rend BR_ASM_DATA rend;
extern _static_cache BR_ASM_DATA scache;

/*
 * Transformation, projection and outcoding in D3D vertex structures
 *
 * General order is
 *
 *		TRANSFORM
 *		OUTCODE (if necessary)
 *		PROJECT
 *
 * Transformation macro temporarily stores W in slot for 1/W in D3DTLVERTEX
 * Projection macro then takes reciprocal and uses it immediately.  Outcoding assumes that W
 * is stored there
 * Outcoding could take place in integer unit (if hand coded) so reciprocal could be calculated
 * immediately after transformation and result used after outcoding
 */


/*
 * Generates outcodes for a homogenous point
 */
#if 1	// Version that uses integer unit to check view volume (using tables would be even better!)
#define OUTCODE_POINT(d3dhv, d3dhw)\
{																				\
	/*                                                                          \
	 * The 6 planes of the view volume...                                       \
	 */                                                                         \
	if (*(d3dhw) >= 0) {                                                        \
	                                                                            \
		if (*(long *)(&(d3dhv)->dvHX) >= *(long *)(d3dhw))                          \
			(d3dhv)->dwFlags ^= (OUTCODE_RIGHT | OUTCODE_N_RIGHT);                  \
		if ((*(long *)(&(d3dhv)->dvHX) ^ (long)0x80000000) > *(long *)(d3dhw)) \
			(d3dhv)->dwFlags ^= (OUTCODE_LEFT | OUTCODE_N_LEFT);                    \
																					\
		if (*(long *)(&(d3dhv)->dvHY) >= *(long *)(d3dhw))                          \
			(d3dhv)->dwFlags ^= (OUTCODE_TOP | OUTCODE_N_TOP);                      \
		if ((*(long *)(&(d3dhv)->dvHY) ^ (long)0x80000000) > *(long *)(d3dhw)) \
			(d3dhv)->dwFlags ^= (OUTCODE_BOTTOM | OUTCODE_N_BOTTOM);                \
																					\
		if (*(long *)(&(d3dhv)->dvHZ) >= *(long *)(d3dhw))                          \
			(d3dhv)->dwFlags ^= (OUTCODE_HITHER | OUTCODE_N_HITHER);                \
		if ((*(long *)(&(d3dhv)->dvHZ) ^ (long)0x80000000) > *(long *)(d3dhw)) \
			(d3dhv)->dwFlags ^= (OUTCODE_YON | OUTCODE_N_YON);                      \
	                                                                            \
	} else {                                                                    \
	                                                                            \
		if (*(long *)(&(d3dhv)->dvHX) < (*(long *)(d3dhw) ^ (long)0x80000000))                          \
			(d3dhv)->dwFlags ^= (OUTCODE_LEFT | OUTCODE_N_LEFT);                    \
		if ((*(long *)(&(d3dhv)->dvHX) ^ (long)0x80000000) <= (*(long *)(d3dhw) ^ (long)0x80000000)) \
			(d3dhv)->dwFlags ^= (OUTCODE_RIGHT | OUTCODE_N_RIGHT);                  \
																					\
		if (*(long *)(&(d3dhv)->dvHY) < (*(long *)(d3dhw) ^ (long)0x80000000))                          \
			(d3dhv)->dwFlags ^= (OUTCODE_BOTTOM | OUTCODE_N_BOTTOM);                \
		if ((*(long *)(&(d3dhv)->dvHY) ^ (long)0x80000000) <= (*(long *)(d3dhw) ^ (long)0x80000000)) \
			(d3dhv)->dwFlags ^= (OUTCODE_TOP | OUTCODE_N_TOP);                      \
																					\
		if (*(long *)(&(d3dhv)->dvHZ) < (*(long *)(d3dhw) ^ (long)0x80000000))                          \
			(d3dhv)->dwFlags ^= (OUTCODE_YON | OUTCODE_N_YON);                      \
		if ((*(long *)(&(d3dhv)->dvHZ) ^ (long)0x80000000) <= (*(long *)(d3dhw) ^ (long)0x80000000)) \
			(d3dhv)->dwFlags ^= (OUTCODE_HITHER | OUTCODE_N_HITHER);                \
	}	                                                                            \
	                                                                            \
	/*                                                                          \
	 * Any user defined clip planes...                                          \
	 */                                                                         \
	if (scache.user_clip_active) {                                              \
		int c;                                                                  \
		for (c = 0; c < MAX_STATE_CLIP_PLANES; c++)                             \
			if (renderer->state.clip[c].type == BRT_PLANE &&                    \
				((d3dhv)->dvHX * BrScalarToFixed(renderer->state.clip[c].plane.v[0]) + \
				(d3dhv)->dvHY * BrScalarToFixed(renderer->state.clip[c].plane.v[1]) + \
				(d3dhv)->dvHZ * BrScalarToFixed(renderer->state.clip[c].plane.v[2]) + \
				*(d3dhw) * BrScalarToFixed(renderer->state.clip[c].plane.v[3])) < 0.0) \
				(d3dhv)->dwFlags ^= (OUTCODE_USER | OUTCODE_N_USER) << c;       \
	}                                                                           \
}
#else
#define OUTCODE_POINT(d3dhv, d3dhw)\
{																				\
	/*																			\
	 * The 6 planes of the view volume...										\
	 */																			\
	if ((d3dhv)->dvHX >= *(d3dhw))										        \
		(d3dhv)->dwFlags ^= (OUTCODE_RIGHT | OUTCODE_N_RIGHT);					\
	if ((d3dhv)->dvHX < -*(d3dhw))										        \
		(d3dhv)->dwFlags ^= (OUTCODE_LEFT | OUTCODE_N_LEFT);					\
																				\
	if ((d3dhv)->dvHY >= *(d3dhw))										        \
		(d3dhv)->dwFlags ^= (OUTCODE_TOP | OUTCODE_N_TOP);						\
	if ((d3dhv)->dvHY < -*(d3dhw))										        \
		(d3dhv)->dwFlags ^= (OUTCODE_BOTTOM | OUTCODE_N_BOTTOM);				\
																				\
	if ((d3dhv)->dvHZ >= *(d3dhw))										        \
		(d3dhv)->dwFlags ^= (OUTCODE_HITHER | OUTCODE_N_HITHER);				\
	if ((d3dhv)->dvHZ < -*(d3dhw))										        \
		(d3dhv)->dwFlags ^= (OUTCODE_YON | OUTCODE_N_YON);						\
																				\
	/*																			\
	 * Any user defined clip planes...											\
	 */																			\
	if (scache.user_clip_active) {												\
		int c;																	\
		for (c = 0; c < MAX_STATE_CLIP_PLANES; c++)								\
			if (renderer->state.clip[c].type == BRT_PLANE &&					\
				((d3dhv)->dvHX * BrScalarToFixed(renderer->state.clip[c].plane.v[0]) + \
				(d3dhv)->dvHY * BrScalarToFixed(renderer->state.clip[c].plane.v[1]) + \
				(d3dhv)->dvHZ * BrScalarToFixed(renderer->state.clip[c].plane.v[2]) + \
				*(d3dhw) * BrScalarToFixed(renderer->state.clip[c].plane.v[3])) < 0.0) \
				(d3dhv)->dwFlags ^= (OUTCODE_USER | OUTCODE_N_USER) << c;		\
	}																			\
}
#endif

/*
 * Generic macro for projecting a vertex from homogenous coordinates
 */
#if 0
#define PROJECT_VERTEX(d3dhv, d3dtlv)\
	{                                                                                          \
		(d3dtlv)->dvSX = scache.sx_offset + scache.sx_scale * (d3dhv)->dvHX * (d3dtlv)->dvRHW; \
		(d3dtlv)->dvSY = scache.sy_offset + scache.sy_scale * (d3dhv)->dvHY * (d3dtlv)->dvRHW; \
		(d3dtlv)->dvSZ = 0.5f - (0.5f * (1.0f - FLT_EPSILON)) * (d3dhv)->dvHZ * (d3dtlv)->dvRHW;                        \
	}
#else
#define PROJECT_VERTEX(d3dhv, d3dtlv)\
	{                                                                                          \
		(d3dtlv)->dvSX = scache.sx_offset + scache.sx_scale * (d3dhv)->dvHX * (d3dtlv)->dvRHW; \
		(d3dtlv)->dvSY = scache.sy_offset + scache.sy_scale * (d3dhv)->dvHY * (d3dtlv)->dvRHW; \
		(d3dtlv)->dvSZ = 0.5f * (1.0f - FLT_EPSILON) - 0.5f * (d3dhv)->dvHZ * (d3dtlv)->dvRHW;                        \
	}
#endif


/*
 * Transform into screen space - Inline expanded BrMatrix4ApplyP()
 *
 * N.B. Currently store W in slot for 1/W temporarily
 */
#define TRANSFORM_VERTEX(d3dhv, d3dhw, d3dtlv, src, mat)\
{                                                  \
	(d3dhv)->dvHX = BrScalarToFloat(BR_MAC3(       \
			(src)->v[X], (mat)->m[0][0],           \
			(src)->v[Y], (mat)->m[1][0],           \
			(src)->v[Z], (mat)->m[2][0]) +         \
					     (mat)->m[3][0]);          \
	                                               \
	(d3dhv)->dvHY = BrScalarToFloat(BR_MAC3(       \
			(src)->v[X], (mat)->m[0][1],           \
			(src)->v[Y], (mat)->m[1][1],           \
			(src)->v[Z], (mat)->m[2][1]) +         \
					     (mat)->m[3][1]);          \
	                                               \
	(d3dhv)->dvHZ = BrScalarToFloat(BR_MAC3(       \
			(src)->v[X], (mat)->m[0][2],           \
			(src)->v[Y], (mat)->m[1][2],           \
			(src)->v[Z], (mat)->m[2][2]) +         \
					     (mat)->m[3][2]);          \
	                                               \
	*(d3dhw) = BrScalarToFloat(BR_MAC3(            \
			(src)->v[X], (mat)->m[0][3],           \
			(src)->v[Y], (mat)->m[1][3],           \
			(src)->v[Z], (mat)->m[2][3]) +         \
					     (mat)->m[3][3]);          \
						                           \
	(d3dtlv)->dvRHW = 1.0f / *(d3dhw);             \
}


/*
 * Alignment for block in scratch area
 */
#define SCRATCH_BOUNDARY 32
#define SCRATCH_ALIGN(x) (((x)+(SCRATCH_BOUNDARY-1)) & ~(SCRATCH_BOUNDARY-1))

/*
 * Update MIN and MAX vertices' x,y
 */
#define UPDATE_BOUNDS(d3dtlv) { \
		if (BrScalarToFloat((d3dtlv)->dvSX) > scache.max.v[X]) scache.max.v[X] = BrScalarToFloat((d3dtlv)->dvSX); \
		if (BrScalarToFloat((d3dtlv)->dvSX) < scache.min.v[X]) scache.min.v[X] = BrScalarToFloat((d3dtlv)->dvSX); \
		if (BrScalarToFloat((d3dtlv)->dvSY) > scache.max.v[Y]) scache.max.v[Y] = BrScalarToFloat((d3dtlv)->dvSY); \
		if (BrScalarToFloat((d3dtlv)->dvSY) < scache.min.v[Y]) scache.min.v[Y] = BrScalarToFloat((d3dtlv)->dvSY); \
	}


/*
 * The level below which the fixed specular power function is zero
 */
#define SPECULARPOW_CUTOFF BR_SCALAR(0.6172)


#ifdef __cplusplus
};
#endif
#endif

