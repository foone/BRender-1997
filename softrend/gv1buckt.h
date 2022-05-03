/*
 * Copyright (c) 1993-1995 Argonaut Technologies Limited. All rights reserved.
 *
 * $Id: gv1buckt.h 2.2 1996/08/02 12:37:51 sam Exp $
 * $Locker: $
 *
 * Geometry format
 */
#ifndef _GV1BUCKT_H_
#define _GV1BUCKT_H_

#ifdef __cplusplus
extern "C" {
#endif

/*
 * Private state of geometry format
 */
typedef struct br_geometry_v1_buckets {
	/*
	 * Dispatch table
	 */
	struct br_geometry_v1_buckets_dispatch *dispatch;

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

} br_geometry_v1_buckets;

#ifdef __cplusplus
};
#endif
#endif



