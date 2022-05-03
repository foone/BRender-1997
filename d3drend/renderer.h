/*
 * Copyright (c) 1993-1995 Argonaut Technologies Limited. All rights reserved.
 *
 * $Id: renderer.h 1.3 1997/08/11 14:25:58 jon Exp JON $
 * $Locker: JON $
 *
 * Private renderer structure
 */
#ifndef _RENDERER_H_
#define _RENDERER_H_

#ifdef __cplusplus
extern "C" {
#endif

#ifdef BR_RENDERER_PRIVATE

/*
 * Renderer - must be compatible with br_renderer_state
 */
typedef struct br_renderer {
	/*
	 * Dispatch table
	 */
	struct br_renderer_dispatch *dispatch;

	/*
	 * Standard object identifier
	 */
	char *identifier;

    /*
     * Pointer to owning device
     */
    struct br_device *device;

	/*
	 * List of objects associated with this renderer
	 */
	void *object_list;

	/*
	 * State of renderer
	 */
	struct state_all state;

	/*
	 * Default state of renderer
	 */
	struct state_all *default_state;

	/*
	 * last restored state
	 */
	struct br_renderer_state_stored *last_restored;

	/*
	 * Output primitives
	 */
	struct br_primitive_library *plib;

	/*
	 * Stack of saved states (XXX should allocate on the fly)
	 */
	struct state_all state_stack[MAX_STATE_STACK];
	br_uint_32 stack_top;

	/*
	 * Type of renderer
	 */
	br_renderer_facility *renderer_facility;

	/*
	 * Direct3D devicec for this renderer
	 */
	LPDIRECT3DDEVICE2 d3d_device;

	/*
	 * Viewport for current rendering target
	 */
	LPDIRECT3DVIEWPORT2 viewport;

	/*
	 * Drawing is in progress
	 */
	br_boolean scene_started;

	/*
	 * Best-fit texture formats for each BRender pixel format
	 */
	struct {

		br_boolean supported;
		br_boolean convert;
		LPDDPIXELFORMAT pixel_format;

	} texture_pixel_type[BR_PMT_MAX];

} br_renderer;

#endif

#ifdef __cplusplus
};
#endif
#endif


