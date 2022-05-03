/*
 * Copyright (c) 1993-1995 Argonaut Technologies Limited. All rights reserved.
 *
 * $Id: Outfcty.h 1.4 1997/08/11 14:25:15 jon Exp JON $
 * $Locker: JON $
 *
 * Private output facility structure
 */
#ifndef _OUTFCTY_H_
#define _OUTFCTY_H_

#ifdef __cplusplus
extern "C" {
#endif


/*
 * Structure containing DirectDraw mode information
 */
typedef struct ddraw_info {

	/*
	 * GUID and text describing this DirectDraw device
	 */
	GUID FAR *guid;
	LPSTR name;
	LPSTR desc;

	/*
	 * DirectDraw interface to the device providing this mode
	 */
	LPDIRECTDRAW2 ddraw;
	
	/*
	 * Capabilities of this DirectDraw device
	 */
	LPDDCAPS hal_caps;
	LPDDCAPS hel_caps;
	
	/*
	 * Properties of this display mode
	 */
	LPDDSURFACEDESC surface_desc;

} ddraw_info;


#ifdef BR_OUTPUT_FACILITY_PRIVATE

/*
 * Private state of output type
 */
typedef struct br_output_facility {
	/*
	 * Dispatch table
	 */
	struct br_output_facility_dispatch *dispatch;

	/*
	 * Standard object identifier
	 */
	char *identifier;

	/*
	 * Pointer to owning device
	 */
	struct br_device *device;

	/*
	 * List of instances associated with type
	 */
	void *object_list;

	/*
	 * Size, minimum and maximum size of mode in pixels
	 */
	br_int_32 width;
	br_int_32 height;	

	br_int_32 min_width;
	br_int_32 min_height;	

	br_int_32 max_width;
	br_int_32 max_height;	

	/*
	 * Bit depth
	 */
	br_int_32 colour_bits;
	br_int_32 depth_bits;

	/*
	 * Pixelmap types
	 */
	br_int_32 colour_type;
	br_int_32 depth_type;

	/*
	 * Is there a CLUT?
	 */
	br_boolean indexed;

	/*
	 * Fullscreen?
	 */
	br_boolean fullscreen;

	/*
	 * Refresh rate in Hz
	 */
	br_uint_32 refresh_rate;

	/*
	 * Renderer facilities to use, and default
	 */
	struct br_renderer_facility **renderer_facility_list;
	struct br_renderer_facility *renderer_facility;

	/*
	 * DirectDraw information about the mode
	 */
	struct ddraw_info ddraw;

} br_output_facility;

#endif

#ifdef __cplusplus
};
#endif
#endif

