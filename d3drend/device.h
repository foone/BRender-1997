/*
 * Copyright (c) 1993-1995 Argonaut Technologies Limited. All rights reserved.
 *
 * $Id: Device.h 1.1 1997/07/11 16:15:01 jon Exp JON $
 * $Locker: JON $
 *
 * Private device driver structure
 */
#ifndef _DEVICE_H_
#define _DEVICE_H_

#ifdef __cplusplus
extern "C" {
#endif

#ifdef BR_OBJECT_PRIVATE

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
     * Driver-wide template store
     */
    struct device_templates templates;

	/*
	 * Handle of DirectDraw library
	 */
	HMODULE ddraw;

	/*
	 * Address of DirectDraw global functions
	 */
	FARPROC DirectDrawCreate;
	FARPROC DirectDrawEnumerate;

	/*
	 * Local copy of CPU capabilities
	 */
	struct host_info hostInfo;

} br_device;

#endif

/*
 * Some useful inline ops.
 */
#define DeviceSoftResource(d) (((br_device *)d)->res)

#ifdef __cplusplus
};
#endif
#endif

