/*
 * Copyright (c) 1993-1995 Argonaut Technologies Limited. All rights reserved.
 *
 * $Id: alpha.c 2.4 1996/11/11 16:07:31 sam Exp $
 * $Locker: $
 *
 * Generation of Alpha component
 */
#include "drv.h"
#include "shortcut.h"
#include "brassert.h"

void SURFACE_CALL SurfaceAlpha(br_renderer *self,
	br_vector3 *p, br_vector2 *map, br_vector3 *n, br_colour colour, br_scalar *comp)
{
	comp[C_A] = BR_MUL(self->state.cache.comp_scales[C_A],
 		BR_CONST_DIV(BrIntToScalar(BR_ALPHA(colour)),256)) +
 		self->state.cache.comp_offsets[C_A];
}

