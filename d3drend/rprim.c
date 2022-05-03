/*
 * Copyright (c) 1993-1995 Argonaut Technologies Limited. All rights reserved.
 *
 * $Id: rprim.c 1.1 1997/07/11 16:14:39 jon Exp JON $
 * $Locker: JON $
 *
 * Geometry format methods
 */
#include <stddef.h>
#include <string.h>

#include "drv.h"
#include "shortcut.h"
#include "brassert.h"

BR_RCS_ID("$Id: rprim.c 1.1 1997/07/11 16:14:39 jon Exp JON $");


br_error BR_CMETHOD_DECL(br_geometry_format_d3d, render_primitives)
		(struct br_geometry *self, struct br_renderer *r)
{
	return BRE_FAIL;
}

br_error BR_CMETHOD_DECL(br_geometry_format_d3d, storedNew_primitives)
		(struct br_geometry *self, struct br_renderer *r)
{
	return BRE_FAIL;
}

br_error BR_CMETHOD_DECL(br_geometry_format_d3d, prepare_primitives)
		(struct br_geometry *self, struct br_renderer *r)
{
	return BRE_FAIL;
}


