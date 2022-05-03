/*
 * Copyright (c) 1993-1995 Argonaut Technologies Limited. All rights reserved.
 *
 * $Id: DRIVER.C 1.2 1997/01/27 19:38:51 STEVEW Exp $
 * $Locker: $
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

BR_RCS_ID("$Id: DRIVER.C 1.2 1997/01/27 19:38:51 STEVEW Exp $");

/*
 * Main entry point for device - this may get redefined by the makefile
 */
br_device * BR_EXPORT BrDrv1Begin(char *arguments)
{
	br_device *dev;

	/*
	 * Set up device
	 */
	dev = DeviceDirectDrawAllocate("DDRAW", arguments);

	if ( !dev )
      return NULL;

	/*
	 * Setup all the available types
	 */
	if(OutputFacilityDirectDrawInitialise(dev) != BRE_OK)
		return NULL;

	/*
	 * If nothing is available, then don't admit to being a device
	 */
	if(dev->output_facility_count == 0)
		return NULL;

	return dev;
}

