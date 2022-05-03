/*
 * Copyright (c) 1993-1995 by Argonaut Technologies Limited. All rights reserved.
 *
 * $Id: stddiag.c 2.3 1996/04/25 14:47:07 sam Exp $
 * $Locker: $
 *
 * Default diagnostic handler that reports through stderr
 */
#include <stdio.h>
#include <stdlib.h>

#include "brender.h"

BR_RCS_ID("$Id: stddiag.c 2.3 1996/04/25 14:47:07 sam Exp $")

static void BR_CALLBACK BrStdioWarning(char *message)
{
	fflush(stdout);
	fputs(message,stderr);
	fputc('\n',stderr);
	fflush(stderr);
}

static void BR_CALLBACK BrStdioFailure(char *message)
{
	/*
	 * Close down all devices etc.
	 */
	BrEnd();

	fflush(stdout);
	fputs(message,stderr);
	fputc('\n',stderr);
	fflush(stderr);
	
	exit(10);
}

/*
 * ErrorHandler structure
 */
br_diaghandler BrStdioDiagHandler = {
	"Stdio DiagHandler",
	BrStdioWarning,
	BrStdioFailure,
};

/*
 * Override default
 */
br_diaghandler * BR_ASM_DATA _BrDefaultDiagHandler = &BrStdioDiagHandler;


