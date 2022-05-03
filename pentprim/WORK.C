/*
 * Copyright (c) 1993-1995 Argonaut Technologies Limited. All rights reserved.
 *
 * $Id: WORK.C 1.2 1996/09/27 16:27:31 JOHNG Exp $
 * $Locker: $
 *
 */
#include "drv.h"
#include "shortcut.h"
#include "brassert.h"

BR_RCS_ID("$Id: WORK.C 1.2 1996/09/27 16:27:31 JOHNG Exp $");

/*
 * The static work area for all the renderers
 */
_prim_work BR_ASM_DATA work;

/*
 * Exported entry point that provides a pointer to the static area for this image
 * 
 * Used to set up autoloaded primitives
 */
struct prim_work * BR_EXPORT PrimLibWork(void)
{
	return &work;
}

