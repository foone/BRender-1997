/*
 * Copyright (c) 1993-1995 Argonaut Technologies Limited. All rights reserved.
 *
 * $Id: object.c 2.2 1996/08/02 12:37:55 sam Exp $
 * $Locker: $
 *
 * Object methods
 */
#include <stddef.h>
#include <string.h>

#include "drv.h"
#include "shortcut.h"
#include "brassert.h"

BR_RCS_ID("$Id: object.c 2.2 1996/08/02 12:37:55 sam Exp $");

/*
 * Methods for default object object
 */
char * BR_CMETHOD_DECL(br_object_soft, identifier)(br_object *self)
{
	return self->identifier;
}

br_device *	BR_CMETHOD_DECL(br_object_soft, device)(br_object *self)
{
	return self->device;
}
