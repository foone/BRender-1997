/*
 * Copyright (c) 1993-1995 Argonaut Technologies Limited. All rights reserved.
 *
 * $Id: DEVPIXMP.H 2.7 1996/03/07 20:40:05 sam Exp $
 * $Locker: $
 *
 * Private device pixelmap structure
 */
#ifndef _DEVPIXMP_H_
#define _DEVPIXMP_H_

#ifdef __cplusplus
extern "C" {
#endif

/*
 * Private state of device pixelmap
 */
typedef struct br_device_pixelmap {
	/*
	 * Dispatch table
	 */
	struct br_device_pixelmap_dispatch *dispatch;

	/*
	 * Standard handle identifier
	 */
	char *pm_identifier;

	/** Standard pixelmap members (not including identifier) **/

	BR_PIXELMAP_MEMBERS

	/** End of br_pixelmap fields **/
	
	struct br_output_facility *output_facility;

	struct br_device_clut *clut;

	/*
	 * Category of pixelmap within device
	 */
	br_int_32 buffer_type;

	/*
	 * Record pointers to matched pixelmaps
	 */
	struct br_device_pixelmap *offscreen;
	struct br_device_pixelmap *depth;

	br_boolean	restore_mode;

} br_device_pixelmap;

#ifdef __cplusplus
};
#endif
#endif


