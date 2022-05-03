/*
 * Copyright (c) 1993-1995 Argonaut Technologies Limited. All rights reserved.
 *
 * $Id: OBJECT.H 1.1 1997/07/11 16:15:05 jon Exp JON $
 * $Locker: JON $
 *
 * Private object structure
 */
#ifndef _OBJECT_H_
#define _OBJECT_H_

#ifdef __cplusplus
extern "C" {
#endif

#ifdef BR_OBJECT_PRIVATE

/*
 * Private state of device pixelmap
 */
typedef struct br_object {
	/*
	 * Dispatch table
	 */
	struct br_object_dispatch *dispatch;

	/*
	 * Standard object identifier
	 */
	char *identifier;

    /*
     * Pointer to owning device
     */
    struct br_device *device;

} br_object;

#endif

/*
 * Override general case with special case for this driver
 */
#define ObjectSoftDevice(d) (((br_object *)d)->device)
#define ObjectSoftIdentifier(d) (((br_object *)d)->identifier)

#ifdef __cplusplus
};
#endif
#endif

