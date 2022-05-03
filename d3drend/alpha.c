/*
 * Copyright (c) 1993-1995 Argonaut Technologies Limited. All rights reserved.
 *
 * $Id: alpha.c 1.1 1997/07/11 16:13:55 jon Exp JON $
 * $Locker: JON $
 *
 * Generation of Alpha component
 */
#include "drv.h"
#include "shortcut.h"
#include "brassert.h"

void SURFACE_CALL SurfaceAlpha(br_renderer *self,
	br_vector3 *p, br_vector2 *map, br_vector3 *n, br_colour colour, D3DTLVERTEX *d3dtlv)
{
	/*
	 * Only called when no lighting
	 *
	 * Makes use of the fact that br_colour and D3DCOLOR are the same!
	 */
	d3dtlv->dcColor = colour | 0x00ffffff;
	d3dtlv->dcSpecular = 0;
}

