/*
 * Copyright (c) 1993-1995 Argonaut Technologies Limited. All rights reserved.
 *
 * $Id: DRIVER.C 1.1 1996/05/13 14:28:37 philip Exp PHILIP $
 * $Locker: PHILIP $
 *
 * Driver interface functions
 */
#include <stddef.h>
#include <string.h>
#include <windows.h>
#include <ddraw.h>

#include "drv.h"
#include "shortcut.h"
#include "brassert.h"

BR_RCS_ID("$Id: DRIVER.C 1.1 1996/05/13 14:28:37 philip Exp PHILIP $");

/*
 * Static device structure
 */
br_device DriverDeviceATI =
{
	NULL, "RAGE_WIN",
};


/* Version number last bumped after delivery 18/2/97 */

/*
 * Main entry point for device - this may get redefined by the makefile
 */
__declspec (dllexport) br_device * BR_EXPORT BrDrv1ATIBegin(char *arguments)
{
	br_device *dev = &DriverDeviceATI;

	/* Set up device */
  dev->object_list = NULL;
  dev->res = NULL;
  dev->output_facility_count = 0;
	if (DeviceATIInitialise(dev) != BRE_OK)
    return(NULL);

	/* Setup DirectDraw output facilities */
  if (OutputFacilityATIInitialise(dev) != BRE_OK)
    return(NULL);

  /* If nothing is available, then don't admit to being a device */
	if (dev->output_facility_count == 0)
		return NULL;
	else
		return dev;
}

