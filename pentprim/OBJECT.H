/*
 * Copyright (c) 1993-1995 Argonaut Technologies Limited. All rights reserved.
 *
 * $Id: OBJECT.H 1.1 1996/09/26 19:03:55 JOHNG Exp $
 * $Locker: $
 *
 * Private object structure
 */
#ifndef _OBJECT_H_
#define _OBJECT_H_

#ifdef __cplusplus
extern "C" {
#endif

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

/*
 * Override general case with special case for this driver
 */
#define ObjectSoftPrimIdentifier(d) (((br_object *)d)->identifier)
#define ObjectSoftPrimDevice(d) (((br_object *)d)->device)

#ifdef __cplusplus
};
#endif
#endif

