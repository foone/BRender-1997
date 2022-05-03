/*
 * Copyright (c) 1993-1995 Argonaut Technologies Limited. All rights reserved.
 *
 * $Id: error.c 2.4 1996/10/01 14:13:52 sam Exp $
 * $Locker: $
 *
 * Error value support
 */

#include "fw.h"

BR_RCS_ID("$Id: error.c 2.4 1996/10/01 14:13:52 sam Exp $")

br_error BR_RESIDENT_ENTRY BrLastErrorGet(void **valuep)
{
	if(valuep)
		valuep = fw.last_error_value;
	
	return fw.last_error_type;
}

void BR_RESIDENT_ENTRY BrLastErrorSet(br_error type, void *value)
{
	fw.last_error_type = type;
	fw.last_error_value = value;
}
