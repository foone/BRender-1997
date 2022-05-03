/*
 * Copyright (c) 1993-1995 Argonaut Technologies Limited. All rights reserved.
 *
 * $Id: defdiag.c 2.2 1996/03/07 20:21:45 sam Exp $
 * $Locker: $
 *
 * Default diagnostic handler that does nothing
 */
#include "brender.h"

BR_RCS_ID("$Id: defdiag.c 2.2 1996/03/07 20:21:45 sam Exp $")

static void BrNullWarning(char *message)
{
}

static void BrNullFailure(char *message)
{
}

/*
 * DiagHandler structure
 */
br_diaghandler BrNullDiagHandler = {
	"Null DiagHandler",
	BrNullWarning,
	BrNullFailure,
};

/*
 * Global variable that can be overridden by linking something first
 */
br_diaghandler *_BrDefaultDiagHandler = &BrNullDiagHandler;

