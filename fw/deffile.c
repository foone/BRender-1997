/*
 * Copyright (c) 1993-1995 Argonaut Technologies Limited. All rights reserved.
 *
 * $Id: deffile.c 2.2 1996/03/07 20:21:46 sam Exp $
 * $Locker: $
 *
 * Default file handler that does nothing
 */
#include "brender.h"

BR_RCS_ID("$Id: deffile.c 2.2 1996/03/07 20:21:46 sam Exp $")

static br_uint_32 BrNullAttributes(void)
{
	return  0;
}

static void BrNullOther(void)
{
	BR_FATAL0("Invald file system call");
}

/*
 * Filesystem structure
 */
br_filesystem BrNullFilesystem = {
	"Null Filesystem",		/* identifier */
	
	BrNullAttributes,		/* attributes */
	(void *)BrNullOther,	/* open_read  */
	(void *)BrNullOther,	/* openwrite  */
	(void *)BrNullOther,	/* close      */
	(void *)BrNullOther,	/* eof		  */

	(void *)BrNullOther,	/* getchar	  */
	(void *)BrNullOther,	/* putchar	  */

	(void *)BrNullOther,	/* read		  */
	(void *)BrNullOther,	/* write	  */

	(void *)BrNullOther,	/* getline	  */
	(void *)BrNullOther,	/* putline	  */

	(void *)BrNullOther,	/* advance	  */
};

/*
 * Global variable that can be overridden by linking something first
 */
br_filesystem *_BrDefaultFilesystem = &BrNullFilesystem;

