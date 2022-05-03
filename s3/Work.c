/*
 * Copyright (c) 1993-1995 Argonaut Technologies Limited. All rights reserved.
 *
 * $Id: work.c 2.1 1996/03/15 17:25:03 sam Exp $
 * $Locker:  $
 *
 */
#include <windows.h>
#include <ddraw.h>

#include "drv.h"
#include "shortcut.h"
#include "brassert.h"

BR_RCS_ID("$Id: work.c 2.1 1996/03/15 17:25:03 sam Exp $");

/*
 * The static work area for all the renderers
 */
struct prim_work BR_ASM_DATA work;

/*
 * Exported entry point that provides a pointer to the static area for this image
 * 
 * Used to set up autoloaded primitives
 */
struct prim_work * BR_EXPORT PrimLibWork(void)
{
	return &work;
}

