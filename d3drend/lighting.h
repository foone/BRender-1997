/*
 * Copyright (c) 1993-1995 Argonaut Technologies Limited. All rights reserved.
 *
 * $Id: lighting.h 1.1 1997/07/11 16:15:29 jon Exp JON $
 * $Locker: JON $
 *
 * Geometry format
 */
#ifndef _GLIGHTING_H_
#define _GLIGHTING_H_

#ifdef __cplusplus
extern "C" {
#endif

#ifdef BR_GEOMETRY_LIGHTING_PRIVATE

/*
 * Private state of geometry format
 */
typedef struct br_geometry_lighting {
	/*
	 * Dispatch table
	 */
        struct br_geometry_lighting_dispatch *dispatch;

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

} br_geometry_lighting;

#endif

#ifdef __cplusplus
};
#endif
#endif



