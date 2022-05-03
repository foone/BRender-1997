/*
 * Copyright (c) 1993-1995 Argonaut Technologies Limited. All rights reserved.
 *
 * $Id: devpixmp.h 1.4 1997/08/11 14:25:32 jon Exp JON $
 * $Locker: JON $
 *
 * Private device pixelmap structure
 */
#ifndef _DEVPIXMP_H_
#define _DEVPIXMP_H_

#ifdef __cplusplus
extern "C" {
#endif

#ifdef BR_DEVICE_PIXELMAP_PRIVATE

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
	
	br_boolean indexed;

	br_boolean restore_mode;
	
	struct br_device *device;
	struct br_output_facility *output_facility;
//	struct br_device_clut *clut;

	/*
	 * Type of buffer (when matched)
	 */
	br_token use;

	/*
	 * Pointer to parent pixelmap (if a sub-pixelmap), the pixelmap it was immediately matched from
	 * and the pixelmap from which it was directly or indirectly matched from
	 */
	struct br_device_pixelmap *parent;
	struct br_device_pixelmap *matched;
	struct br_device_pixelmap *screen;

	/*
	 * Gross hack to get around the fact that you cannot initialise the renderer without offscreen
	 * and depth buffers already allocated
	 */
	LPDIRECTDRAWSURFACE2 offscreen_surface;
	LPDIRECTDRAWSURFACE2 depth_surface;

	/*
	 * Pointer to renderer currently opened on this pixelmap (N.B. This is only set on the screen
	 * pixelmap)
	 */
	struct br_renderer *renderer;

	/*
	 * Renderer is currently drawing to on this pixelmap
	 */
	br_boolean renderer_active;

	/*
	 * Handle to associated window
	 */
	HWND window;
	
	/*
	 * Pointer to DirectDraw surface
	 */
	LPDIRECTDRAWSURFACE2 surface;

	/*
	 * Pointer to DirectDraw clipper, if appropriate (used on primary surfaces)
	 */
	LPDIRECTDRAWCLIPPER clipper;

} br_device_pixelmap;

#endif

#ifdef __cplusplus
};
#endif
#endif


