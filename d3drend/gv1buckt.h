/*
 * Copyright (c) 1993-1995 Argonaut Technologies Limited. All rights reserved.
 *
 * $Id: gv1buckt.h 1.1 1997/07/11 16:15:26 jon Exp JON $
 * $Locker: JON $
 *
 * Geometry format
 */
#ifndef _GV1BUCKT_H_
#define _GV1BUCKT_H_

#ifdef __cplusplus
extern "C" {
#endif

#ifdef BR_GEOMETRY_V1_BUCKETS_PRIVATE

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

#endif

#ifdef __cplusplus
};
#endif
#endif



