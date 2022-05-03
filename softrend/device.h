/*
 * Copyright (c) 1993-1995 Argonaut Technologies Limited. All rights reserved.
 *
 * $Id: device.h 2.5 1997/08/13 11:37:15 JOHNG Exp $
 * $Locker: $
 *
 * Private device driver structure
 */
#ifndef _DEVICE_H_
#define _DEVICE_H_

#include "host.h"


#ifdef __cplusplus
extern "C" {
#endif
/*
 * Private state of device
 */
typedef struct br_device {
	/*
	 * Dispatch table
	 */
	struct br_device_dispatch *dispatch;

	/*
	 * Standard object identifier
	 */
	char *identifier;

    /*
     * Pointer to owning device
     */
    struct br_device *device;

	/*
	 * List of objects associated with this device
	 */
	void *object_list;

	/*
	 * Anchor for all device's resources
	 */
	void *res;

    /*
     * Driver-wide template store
     */
    struct device_templates templates;

// local copy of cpu capabilities
    struct host_info hostInfo;


} br_device;

/*
 * Some useful inline ops.
 */
#define DeviceSoftResource(d) (((br_device *)d)->res)

#ifdef __cplusplus
};
#endif
#endif

