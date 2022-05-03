/*
 * Copyright (c) 1993-1995 Argonaut Technologies Limited. All rights reserved.
 *
 * $Id: MATCH.H 1.1 1997/07/11 16:15:04 jon Exp JON $
 * $Locker: JON $
 *
 * Bits used to match renderer state to primitives
 */
#ifndef _MATCH_H_
#define _MATCH_H_

#ifdef __cplusplus
extern "C" {
#endif

/*
 * Local version of brp_info extended with information used to match primitve with current state
 */
struct local_block {
	brp_block	p;

	/*
	 * Mask of match flags
	 */
	br_uint_32	flags_mask;
	br_uint_32	flags_cmp;

	/*
	 * Type of maps supported, if any
	 */
	br_int_32	depth_type;
	br_int_32	texture_type;
	br_int_32	shade_type;
	br_int_32	blend_type;
	br_int_32	screendoor_type;
	br_int_32	lighting_type;
	br_int_32	bump_type;

	/*
	 * Used to check if the colour.width_b's of the two pixelmaps match -
	 *
	 *	if colour colour.width_b * depth_row_size != depth colour.width_b * colour_row_size
	 *	then this primtiive will not match
	 */
	br_uint_8	colour_row_size;
	br_uint_8	depth_row_size;

	/*
	 * Texture dimensions required (0 if don't care)
	 */
	br_int_32	map_width;
	br_int_32	map_height;

	/*
	 * Type of input colour (indexed or RGB)
	 */
	br_token	input_colour_type;

	/*
	 * Default offset and scale for R,G,B,A
	 */
	br_scalar	colour_offsets[4];
	br_scalar	colour_scales[4];

	/* Flags for information about how ranges are constructed
	 */
	br_uint_32 range_flags;

	/*
	 * pointer to static parameter work area
	 */
	struct prim_work *work;

	/*
	 * Information for autoloading
	 */
	char	*image_name;
	char	*entry_name;

	/*
	 * Next block for chained blocks (FORCE_FRONT, DECAL etc.)
	 */
	struct local_prim_info *chain;
};

/*
 * Valid range flags
 */
enum {
	RF_DECAL		= 0x0008,
	RF_RGB_SHADE	= 0x0010,
};

#ifdef __cplusplus
};
#endif
#endif

