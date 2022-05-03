/*
 * Copyright (c) 1993-1995 Argonaut Technologies Limited. All rights reserved.
 *
 * $Id: OBJECT.C 1.1 1996/09/26 19:03:55 JOHNG Exp $
 * $Locker: $
 *
 * Object methods
 */
#include <stddef.h>
#include <string.h>

#include "drv.h"
#include "shortcut.h"
#include "brassert.h"

BR_RCS_ID("$Id: OBJECT.C 1.1 1996/09/26 19:03:55 JOHNG Exp $");

/*
 * Methods for default object object
 */
char * BR_CMETHOD_DECL(br_object_softprim, identifier)(br_object *self)
{
	return self->identifier;
}

br_device *	BR_CMETHOD_DECL(br_object_softprim, device)(br_object *self)
{
	return self->device;
}
