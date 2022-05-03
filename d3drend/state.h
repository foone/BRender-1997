/*
 * Copyright (c) 1993-1995 Argonaut Technologies Limited. All rights reserved.
 *
 * $Id: state.h 1.2 1997/08/11 14:26:09 jon Exp JON $
 * $Locker: JON $
 *
 * Renderer state info
 */
#ifndef _STATE_H_
#define _STATE_H_

#ifdef __cplusplus
extern "C" {
#endif

/*
 * Some pre-defined limits
 */
#define MAX_STATE_STACK			32
#define MAX_STATE_LIGHTS		16
#define MAX_STATE_CLIP_PLANES	6

/*
 * Bits used to create a mask for each part of the state
 *
 * There are some global defines for these, so use them
 */
enum {
	MASK_STATE_CULL			= BR_STATE_CULL,
	MASK_STATE_SURFACE		= BR_STATE_SURFACE,
	MASK_STATE_MATRIX		= BR_STATE_MATRIX,
	MASK_STATE_ENABLE		= BR_STATE_ENABLE,
	MASK_STATE_LIGHT		= BR_STATE_LIGHT,
	MASK_STATE_CLIP			= BR_STATE_CLIP,
	MASK_STATE_BOUNDS		= BR_STATE_BOUNDS,
	MASK_STATE_PRIMITIVE	= BR_STATE_PRIMITIVE,
	MASK_STATE_OUTPUT		= BR_STATE_OUTPUT,
	MASK_STATE_CACHE		= BR_STATE_CACHE,

	MASK_STATE_LOCAL	= (	MASK_STATE_CULL|
							MASK_STATE_SURFACE|
							MASK_STATE_MATRIX|
							MASK_STATE_ENABLE|
							MASK_STATE_LIGHT|
							MASK_STATE_CLIP|
							MASK_STATE_BOUNDS|
							MASK_STATE_PRIMITIVE|
							MASK_STATE_OUTPUT|
							MASK_STATE_CACHE)
};

/*
 * state.enable.flags
 */
enum {
	ENBL_CULL_BIT,
	ENBL_TRANSFORM_BIT,
	ENBL_SURFACE_BIT,
	ENBL_CLIPPING_BIT,
	ENBL_BOUNDS_BIT,
	ENBL_RENDER_BIT,
};

enum {
	ENBL_CULL		= (1<<ENBL_CULL_BIT),
	ENBL_TRANSFORM	= (1<<ENBL_TRANSFORM_BIT),
	ENBL_SURFACE	= (1<<ENBL_SURFACE_BIT),
	ENBL_CLIPPING	= (1<<ENBL_CLIPPING_BIT),
	ENBL_BOUNDS		= (1<<ENBL_BOUNDS_BIT),
	ENBL_RENDER		= (1<<ENBL_RENDER_BIT)
};

/*
 * Culling information
 */
struct state_cull {
	br_token	type;
	br_token	space;

	br_timestamp	timestamp;
};

/*
 * Surface information
 */
struct state_surface {
	br_colour	colour;

	br_scalar	opacity;
	br_scalar	ka;
	br_scalar	kd;
	br_scalar	ks;
	br_scalar	power;

	br_boolean	lighting;
	br_boolean	force_front;
	br_boolean	force_back;

	br_token	colour_source;
	br_token	mapping_source;
	
	br_matrix23	map_transform;

	br_timestamp	timestamp;
	br_timestamp	timestamp_transform;
};

/*
 * Light information
 */
struct state_light {
	br_token	type;
	br_token	lighting_space;

	br_vector3	position;
	br_vector3	direction;

	br_colour	colour;

	br_scalar	spot_outer;
	br_scalar	spot_inner;

	br_scalar	attenuation_l;
	br_scalar	attenuation_c;
	br_scalar	attenuation_q;

	br_timestamp	timestamp;
};

/*
 * Clipping information
 */
struct state_clip {
	br_token	type;
	br_vector4	plane;
};

/*
 * Matrix information
 */
struct state_matrix {
	br_matrix34	model_to_view;
	br_matrix4	view_to_screen;
	br_matrix34	view_to_environment;

	br_token	model_to_view_hint;
	br_token	view_to_screen_hint;
	br_token	view_to_environment_hint;

	br_scalar	hither_z;
	br_scalar	yon_z;
	
	br_timestamp	timestamp;
	br_timestamp	timestamp_v2s_hint;
	br_timestamp	timestamp_m2v;
	br_timestamp	timestamp_v2s;
};

/*
 * Bounds information
 */
struct state_bounds {
	br_vector2	min;
	br_vector2	max;
};

/*
 * Enables
 */
struct state_enable {
	br_uint_32	flags;

	br_timestamp	timestamp;
};

/*
 * Hidden surface info.
 */
typedef void BR_CALLBACK insert_cbfn(
	br_primitive *primitive,
	void *arg1, void *arg2, void *arg3,
	br_order_table *order_table, br_scalar *z);

struct state_hidden {
	br_token			type;
	br_order_table		*order_table;
	br_primitive_heap	*heap;
	insert_cbfn			*insert_fn;
	void				*insert_arg1;
	void				*insert_arg2;
	void				*insert_arg3;

	br_timestamp		timestamp;
};

/*
 * state.prim.flags
 */
enum {
	/*
	 * The public flags
	 */
	PRIMF_FORCE_FRONT_BIT,
	PRIMF_SMOOTH_BIT,
	PRIMF_DECAL_BIT,

	PRIMF_DITHER_COLOUR_BIT,
	PRIMF_DITHER_MAP_BIT,
	PRIMF_DEPTH_WRITE_BIT,
	PRIMF_COLOUR_WRITE_BIT,

	PRIMF_INDEXED_COLOUR_BIT,
	PRIMF_BLEND_BIT,
	PRIMF_MODULATE_BIT,

	PRIMF_FOG_BIT,

	/*
	 * Internal flags using for matching
	 */
	PRIMF_OPAQUE_MAP_BIT,
	PRIMF_NO_SKIP_BIT,
	PRIMF_PERSPECTIVE_BIT,
	PRIMF_POWER2_BIT,
	PRIMF_STRIDE_POSITIVE_BIT,
	PRIMF_PALETTE_BIT,
	PRIMF_RANGE_ZERO_BIT,
};

enum {
	PRIMF_FORCE_FRONT		= (1<<PRIMF_FORCE_FRONT_BIT),
	PRIMF_SMOOTH			= (1<<PRIMF_SMOOTH_BIT),
	PRIMF_DECAL             = (1<<PRIMF_DECAL_BIT),
	PRIMF_DITHER_COLOUR		= (1<<PRIMF_DITHER_COLOUR_BIT),
	PRIMF_DITHER_MAP		= (1<<PRIMF_DITHER_MAP_BIT),
	PRIMF_DEPTH_WRITE		= (1<<PRIMF_DEPTH_WRITE_BIT),
	PRIMF_COLOUR_WRITE      = (1<<PRIMF_COLOUR_WRITE_BIT),
	PRIMF_INDEXED_COLOUR    = (1<<PRIMF_INDEXED_COLOUR_BIT),
	PRIMF_BLEND             = (1<<PRIMF_BLEND_BIT),
	PRIMF_MODULATE		   	= (1<<PRIMF_MODULATE_BIT),
	PRIMF_FOG				= (1<<PRIMF_FOG_BIT),

	PRIMF_OPAQUE_MAP		= (1<<PRIMF_OPAQUE_MAP_BIT),
	PRIMF_NO_SKIP			= (1<<PRIMF_NO_SKIP_BIT),
	PRIMF_PERSPECTIVE		= (1<<PRIMF_PERSPECTIVE_BIT),
	PRIMF_POWER2			= (1<<PRIMF_POWER2_BIT),
	PRIMF_STRIDE_POSITIVE	= (1<<PRIMF_STRIDE_POSITIVE_BIT),
	PRIMF_PALETTE			= (1<<PRIMF_PALETTE_BIT),

	PRIMF_INTERNAL			= (	PRIMF_OPAQUE_MAP|
								PRIMF_NO_SKIP|
								PRIMF_PERSPECTIVE|
								PRIMF_POWER2|
								PRIMF_PALETTE),

	PRIMF_RANGE_ZERO		= (1<<PRIMF_RANGE_ZERO_BIT),
};

/*
 * PRIMITIVE part of state
 */
struct state_prim {
	/*
	 * Timestamp for any change of this state
	 */
	br_timestamp timestamp;

	/*
	 * Timestamp for any changes other than input buffer addresses
	 */
	br_timestamp timestamp_major;

	/*
	 * flags
	 */
	br_uint_32 flags;

	/*
	 * User selected ramp
	 */
	br_int_32	index_base;
	br_int_32	index_range;

	/*
	 * Type of colour
	 */
	br_token	colour_type;

	/*
	 * Type of perspective correct rendering
	 */
	br_token	perspective_type;
	br_int_32	subdivide_tolerance;

	/*
	 * Current input buffers
	 */
	struct br_buffer_stored *colour_map;
	struct br_buffer_stored *index_shade;
	struct br_buffer_stored *index_blend;
	struct br_buffer_stored *index_fog;
	struct br_buffer_stored *screendoor;
	struct br_buffer_stored *lighting;
	struct br_buffer_stored *bump;

	/*
	 * Fog flag
	 */
	br_token        fog_type;
};

/*
 * OUTPUT part of state
 */
struct state_output {
	/*
	 * Timestamp for any change of this state
	 */
	br_timestamp timestamp;

	/*
	 * Timestamp for any changes other than input buffer addresses
	 */
	br_timestamp timestamp_major;

	/*
	 * Current output buffers
	 */
	struct br_device_pixelmap *colour;
	struct br_device_pixelmap *depth;

};

/*
 * Cache of info that is copies with STATE_CACHE
 */
#define GEOMETRY_CALL BR_ASM_CALL
#define SURFACE_CALL BR_ASM_CALL

typedef void GEOMETRY_CALL geometry_fn(struct br_geometry *self, struct br_renderer *renderer);
typedef void SURFACE_CALL surface_fn(struct br_renderer *self,
	br_vector3 *p, br_vector2 *map, br_vector3 *n, br_colour colour, LPD3DTLVERTEX v);

#define MAX_GEOMETRY_FNS 16
#define MAX_SURFACE_FNS 8
#define MAX_FACE_BLOCKS 6

struct state_cache {
	/*
	 * mask of components that need clipping
	 */
	br_uint_32 clip_slots;

	/*
	 * Cached geometry & surface functions
	 */
	br_int_32 ngeometry_fns;
	geometry_fn *geometry_fns[MAX_GEOMETRY_FNS];

	br_int_32 ngeometry_fns_onscreen;
	geometry_fn *geometry_fns_onscreen[MAX_GEOMETRY_FNS];

	/*
	 * Minimal primitive blocks for extra per-face operations
	 */
	brp_block_min face_blocks[MAX_FACE_BLOCKS];
	brp_block_min face_blocks_onscreen[MAX_FACE_BLOCKS];
	br_int_8 nface_blocks;
	br_int_8 nface_blocks_onscreen;

	struct br_geometry *format;

	/*
	 * Cached surface functions
	 */
	br_int_32 nvertex_fns;
	surface_fn *vertex_fns[MAX_SURFACE_FNS];

	br_int_32 nconstant_fns;
	surface_fn *constant_fns[MAX_SURFACE_FNS];

	/*
	 * Full map_transform (including U,V ranges)
	 */
	br_matrix23	map_transform;

	/*
	 * Amounts to shift U & V, if map_transform is simple
	 */
	br_int_8 u_shift;
	br_int_8 v_shift;

	/*
	 * Transformed mapping coordinates for corners of unit quad
	 */
	br_vector2 quad_transformed[4];

	/*
	 * Flag for whether cache is valid
	 */
	br_boolean valid;
};

/*
 * Wrapper with all state
 */
typedef struct state_all {
	/*
	 * Pointer to renderer that this state is asociated with
	 */
	struct br_renderer *renderer;

	/*
	 * mask of valid parts
	 */
	br_uint_32 valid;

	struct state_cull cull;
	struct state_surface surface;

	struct state_light light[MAX_STATE_LIGHTS];
	br_timestamp timestamp_lights;

	struct state_clip clip[MAX_STATE_CLIP_PLANES];
	br_timestamp timestamp_clips;

	struct state_matrix matrix;
	struct state_bounds bounds;
	struct state_enable enable;
	struct state_hidden hidden;
	struct state_prim prim;
	struct state_output out;
	struct state_cache cache;

	br_timestamp timestamp_copy_cache;
	br_timestamp timestamp_cache;

} state_all;

#ifdef __cplusplus
};
#endif
#endif

