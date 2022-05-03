/*
 * Copyright (c) 1993-1995 Argonaut Technologies Limited. All rights reserved.
 *
 * $Id: Driver.c 1.4 1997/08/11 14:25:02 jon Exp JON $
 * $Locker: JON $
 *
 * Driver interface functions
 */
#include <stddef.h>
#include <string.h>

/*
 * Define GUIDs for driver-wide use
 */
#ifdef __DRIVER__
#define INITGUID
#endif

#include "drv.h"
#include "shortcut.h"
#include "brassert.h"


BR_RCS_ID("$Id: Driver.c 1.4 1997/08/11 14:25:02 jon Exp JON $");


/*
 * Driver-wide timestamp - the only static variable
 */
br_uint_32 DriverTimestamp;


/*
 * Enumeration of DirectDraw devices
 */
struct enumContext {

	br_device *device;

	br_uint_32 output_facility_count;
};


static BOOL FAR PASCAL enumDDrawDevice(GUID FAR *guid, LPSTR desc, LPSTR name, struct enumContext *context);
static br_boolean suitableDDrawDevice(LPDDCAPS hal_caps, LPDDCAPS hel_caps);


/*
 * Main entry point for device - this may get redefined by the makefile
 */
br_device * BR_EXPORT BrDrv1Begin(char *arguments)
{
	struct enumContext context;

	/*
	 * Setup static timestamp
	 */
	if (DriverTimestamp == 0)
		DriverTimestamp = TIMESTAMP_START;

	/*
	 * Set up device
	 */
	context.device = DeviceD3DAllocate("d3drend", arguments);
	if(context.device == NULL)
		return NULL;

	/*
	 * Enumerate all DirectDraw devices in the machine and initialise renderer facilities and
	 * output facilities for each
	 *
	 * Fail driver initialisation if no valid modes were found
	 */
	context.output_facility_count = 0;
	if (context.device->DirectDrawEnumerate(enumDDrawDevice, (LPVOID)&context) != DD_OK ||
		context.output_facility_count == 0) {

		ObjectFree(context.device);
		return NULL;
	}

	return context.device;
}


BOOL FAR PASCAL enumDDrawDevice(GUID FAR *guid, LPSTR desc, LPSTR name, struct enumContext *context)
{
	LPDIRECTDRAW ddraw_device;
	ddraw_info ddraw;
	br_renderer_facility *renderer_facility_list[33];
	br_uint_32 renderer_facility_count, output_facility_count;
	br_uint_32 i;

	/*
	 * Create an instance of this DirectDraw device
	 */
	if (context->device->DirectDrawCreate(guid, &ddraw_device, NULL) != DD_OK)
		return DDENUMRET_OK;

	/* 
	 * Get the newer version of the DirectDraw interface
	 */
	if (IDirectDraw_QueryInterface(ddraw_device, &IID_IDirectDraw2, &ddraw.ddraw) != DD_OK) {
		IDirectDraw_Release(ddraw_device);
		return DDENUMRET_OK;
	}

	IDirectDraw_Release(ddraw_device);

	/*
	 * Fetch the capabilities of the device
	 */
	ddraw.hal_caps = BrResAllocate(context->device, sizeof(*ddraw.hal_caps), BR_MEMORY_OBJECT_DATA);
	if (ddraw.hal_caps == NULL) {
		IDirectDraw2_Release(ddraw.ddraw);
		return DDENUMRET_OK;
	}

	ddraw.hel_caps = BrResAllocate(context->device, sizeof(*ddraw.hel_caps), BR_MEMORY_OBJECT_DATA);
	if (ddraw.hel_caps == NULL) {
		BrResFree(ddraw.hal_caps);
		IDirectDraw2_Release(ddraw.ddraw);
		return DDENUMRET_OK;
	}

	ddraw.hal_caps->dwSize = sizeof(*ddraw.hal_caps);
	ddraw.hel_caps->dwSize = sizeof(*ddraw.hel_caps);

	if (IDirectDraw2_GetCaps(ddraw.ddraw, ddraw.hal_caps, ddraw.hel_caps) != DD_OK) {
		BrResFree(ddraw.hel_caps);
		BrResFree(ddraw.hal_caps);
		IDirectDraw2_Release(ddraw.ddraw);
		return DDENUMRET_OK;
	}

	/*
	 * Determine if the device is interesting
	 */
	if (!suitableDDrawDevice(ddraw.hal_caps, ddraw.hel_caps)) {
		BrResFree(ddraw.hel_caps);
		BrResFree(ddraw.hal_caps);
		IDirectDraw2_Release(ddraw.ddraw);
		return DDENUMRET_OK;
	}

	/*
	 * Copy the GUID, name and description, attaching them as resources to
	 * the device
	 */
	ddraw.name = BrResStrDup(context->device, name);
	ddraw.desc = BrResStrDup(context->device, desc);

	if (guid != NULL) {
	
		ddraw.guid = BrResAllocate(context->device, sizeof(*ddraw.guid), BR_MEMORY_OBJECT_DATA);
		if (ddraw.guid == NULL)
			return DDENUMRET_OK;

		BrMemCpy(ddraw.guid, guid, sizeof(*ddraw.guid));

	} else

		ddraw.guid = NULL;
		
	/*
	 * Initialise renderer facilities for this device
	 */
	if (RendererFacilityD3DInitialise(context->device, &ddraw, renderer_facility_list, BR_ASIZE(renderer_facility_list),
		&renderer_facility_count) != BRE_OK || renderer_facility_count == 0) {

		if (ddraw.guid != NULL)
			BrResFree(ddraw.guid);
		BrResFree(ddraw.desc);
		BrResFree(ddraw.name);
		BrResFree(ddraw.hel_caps);
		BrResFree(ddraw.hal_caps);
		IDirectDraw2_Release(ddraw.ddraw);
		return DDENUMRET_OK;
	}

	/*
	 * Initialise output facilities for this device
	 */
	if (OutputFacilityD3DInitialise(context->device, &ddraw, renderer_facility_list, renderer_facility_count,
		&output_facility_count) != BRE_OK || output_facility_count == 0) {

		for (i = 0; i < renderer_facility_count; i++)
			ObjectFree(renderer_facility_list[i]);
		if (ddraw.guid != NULL)
			BrResFree(ddraw.guid);
		BrResFree(ddraw.desc);
		BrResFree(ddraw.name);
		BrResFree(ddraw.hel_caps);
		BrResFree(ddraw.hal_caps);
		IDirectDraw2_Release(ddraw.ddraw);
		return DDENUMRET_OK;
	}

	context->output_facility_count += output_facility_count;

	/*
	 * Release the interface used for enumeration (N.B. New references
	 * will have been added for each renderer facility and/or output facility)
	 */
	IDirectDraw2_Release(ddraw.ddraw);

	return DDENUMRET_OK;
}


/*
 * Predicate for whether a given DirectDraw device is suitable as an output facility
 */
static br_boolean suitableDDrawDevice(LPDDCAPS hal_caps, LPDDCAPS hel_caps)
{
	return BR_TRUE;
}
