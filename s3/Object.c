/*
 * Copyright (c) 1993-1995 Argonaut Technologies Limited. All rights reserved.
 *
 * $Id: OBJECT.C 1.1 1996/05/13 14:28:41 philip Exp PHILIP $
 * $Locker: PHILIP $
 *
 * Local methods for default object object
 *
 */
#include <stddef.h>
#include <string.h>
#include <windows.h>
#include <ddraw.h>

#include "drv.h"
#include "shortcut.h"
#include "brassert.h"

BR_RCS_ID("$Id: OBJECT.C 1.1 1996/05/13 14:28:41 philip Exp PHILIP $");

/*
 * Get the identifier associated with a object
 */
char * BR_CMETHOD_DECL(br_object_s3, identifier)(br_object *self)
{
	return self->identifier;
}

/*
 * Find the device associated with a object
 */
br_device *	BR_CMETHOD_DECL(br_object_s3, device)(br_object *self)
{
	return (br_device *)&DriverDeviceS3;
}


