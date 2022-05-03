/*
 * Copyright (c) 1993-1995 Argonaut Technologies Limited. All rights reserved.
 *
 * $Id: OBJECT.C 1.1 1997/07/11 16:13:48 jon Exp JON $
 * $Locker: JON $
 *
 * Object methods
 */
#include <stddef.h>
#include <string.h>

#include "drv.h"
#include "shortcut.h"
#include "brassert.h"

BR_RCS_ID("$Id: OBJECT.C 1.1 1997/07/11 16:13:48 jon Exp JON $");

/*
 * Methods for default object object
 */
char * BR_CMETHOD_DECL(br_object_d3d, identifier)(br_object *self)
{
	return self->identifier;
}

br_device *	BR_CMETHOD_DECL(br_object_d3d, device)(br_object *self)
{
	return self->device;
}
