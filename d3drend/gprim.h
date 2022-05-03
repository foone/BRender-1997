/*
 * Copyright (c) 1993-1995 Argonaut Technologies Limited. All rights reserved.
 *
 * $Id: gprim.h 1.1 1997/07/11 16:15:25 jon Exp JON $
 * $Locker: JON $
 *
 * Geometry format
 */
#ifndef _GPRIM_H_
#define _GPRIM_H_

#ifdef __cplusplus
extern "C" {
#endif

#ifdef BR_GEOMETRY_PRIMITIVES_PRIVATE

/*
 * Private state of geometry format
 */
typedef struct br_geometry_primitives {
	/*
	 * Dispatch table
	 */
	struct br_geometry_primitives_dispatch *dispatch;

	/*
	 * Standard object identifier
	 */
	char *identifier;

    /*
     * Pointer to owning device
     */
    struct br_device *device;

	/*
	 * Renderer type this format is associated with
	 */
	struct br_renderer_facility *renderer_facility;

} br_geometry_primitives;

#endif

#ifdef __cplusplus
};
#endif
#endif



