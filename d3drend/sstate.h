/*
 * Copyright (c) 1993-1995 Argonaut Technologies Limited. All rights reserved.
 *
 * $Id: sstate.h 1.2 1997/08/11 14:26:07 jon Exp JON $
 * $Locker: JON $
 *
 * Stored renderer state
 */
#ifndef _SSTATE_H_
#define _SSTATE_H_

#ifdef __cplusplus
extern "C" {
#endif

/*
 * Parts of state that are stored
 */
#define MASK_STATE_STORED (	MASK_STATE_CULL|\
						  	MASK_STATE_SURFACE|\
						  	MASK_STATE_PRIMITIVE|\
						  	MASK_STATE_CACHE)

#ifdef BR_RENDERER_STATE_STORED_PRIVATE

/*
 * Private state of geometry format
 */
typedef struct br_renderer_state_stored {
	/*
	 * Dispatch table
	 */
	struct br_renderer_state_stored_dispatch *dispatch;

	/*
	 * Standard object identifier
	 */
	char *identifier;

    /*
     * Pointer to owning device
     */
    struct br_device *device;

	/*
	 * Saved state
	 */

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
	struct state_prim prim;
	struct state_cache cache;

	br_timestamp timestamp_copy_cache;
	br_timestamp timestamp_cache;

} br_renderer_state_stored;

#endif

#ifdef __cplusplus
};
#endif
#endif



