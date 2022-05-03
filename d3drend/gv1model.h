/*
 * Copyright (c) 1993-1995 Argonaut Technologies Limited. All rights reserved.
 *
 * $Id: gv1model.h 1.1 1997/07/11 16:15:27 jon Exp JON $
 * $Locker: JON $
 *
 * Geometry format
 */
#ifndef _GV1MODEL_H_
#define _GV1MODEL_H_

#ifdef __cplusplus
extern "C" {
#endif

#ifdef BR_GEOMETRY_V1_MODEL_PRIVATE

/*
 * Private state of geometry format
 */
typedef struct br_geometry_v1_model {
	/*
	 * Dispatch table
	 */
	struct br_geometry_v1_model_dispatch *dispatch;

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

} br_geometry_v1_model;

#endif

#ifdef __cplusplus
};
#endif
#endif


