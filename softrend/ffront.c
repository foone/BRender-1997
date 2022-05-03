/*
 * Copyright (c) 1993-1995 Argonaut Technologies Limited. All rights reserved.
 *
 * $Id: ffront.c 2.1 1996/01/19 20:56:40 sam Exp $
 * $Locker: $
 *
 * Primitive renderer filters that will call primitives with near SZ values
 */
#include <stddef.h>
#include <string.h>

#include "drv.h"
#include "shortcut.h"
#include "brassert.h"

BR_RCS_ID("$Id: ffront.c 2.1 1996/01/19 20:56:40 sam Exp $");

void BR_ASM_CALL RenderForceFront1(struct brp_block *block,
	brp_vertex *v0)
{
	brp_vertex v[1];

	v[0] = *v0;

	v[0].comp[C_SZ] = renderer->state.cache.comp_offsets[C_SZ]-renderer->state.cache.comp_scales[C_SZ];

	block->chain->render(block->chain, v+0);
}

void BR_ASM_CALL RenderForceBack1(struct brp_block *block,
	brp_vertex *v0)
{
	brp_vertex v[1];

	v[0] = *v0;

	v[0].comp[C_SZ] = renderer->state.cache.comp_offsets[C_SZ]+renderer->state.cache.comp_scales[C_SZ];

	block->chain->render(block->chain, v+0);
}

void BR_ASM_CALL RenderForceFront2(struct brp_block *block,
	brp_vertex *v0, brp_vertex *v1)
{
	brp_vertex v[2];

	v[0] = *v0;
	v[1] = *v1;

	v[0].comp[C_SZ] = renderer->state.cache.comp_offsets[C_SZ]-renderer->state.cache.comp_scales[C_SZ];
	v[1].comp[C_SZ] = renderer->state.cache.comp_offsets[C_SZ]-renderer->state.cache.comp_scales[C_SZ];

	block->chain->render(block->chain, v+0, v+1);
}

void BR_ASM_CALL RenderForceBack2(struct brp_block *block,
	brp_vertex *v0, brp_vertex *v1)
{
	brp_vertex v[2];

	v[0] = *v0;
	v[1] = *v1;

	v[0].comp[C_SZ] = renderer->state.cache.comp_offsets[C_SZ]+renderer->state.cache.comp_scales[C_SZ];
	v[1].comp[C_SZ] = renderer->state.cache.comp_offsets[C_SZ]+renderer->state.cache.comp_scales[C_SZ];

	block->chain->render(block->chain, v+0, v+1);
}

void BR_ASM_CALL RenderForceFront3(struct brp_block *block,
	brp_vertex *v0, brp_vertex *v1,brp_vertex *v2)
{
	brp_vertex v[3];

	v[0] = *v0;
	v[1] = *v1;
	v[2] = *v2;

	v[0].comp[C_SZ] = renderer->state.cache.comp_offsets[C_SZ]-renderer->state.cache.comp_scales[C_SZ];
	v[1].comp[C_SZ] = renderer->state.cache.comp_offsets[C_SZ]-renderer->state.cache.comp_scales[C_SZ];
	v[2].comp[C_SZ] = renderer->state.cache.comp_offsets[C_SZ]-renderer->state.cache.comp_scales[C_SZ];

	block->chain->render(block->chain, v+0, v+1, v+2);
}

void BR_ASM_CALL RenderForceBack3(struct brp_block *block,
	brp_vertex *v0, brp_vertex *v1,brp_vertex *v2)
{
	brp_vertex v[3];

	v[0] = *v0;
	v[1] = *v1;
	v[2] = *v2;

	v[0].comp[C_SZ] = renderer->state.cache.comp_offsets[C_SZ]+renderer->state.cache.comp_scales[C_SZ];
	v[1].comp[C_SZ] = renderer->state.cache.comp_offsets[C_SZ]+renderer->state.cache.comp_scales[C_SZ];
	v[2].comp[C_SZ] = renderer->state.cache.comp_offsets[C_SZ]+renderer->state.cache.comp_scales[C_SZ];

	block->chain->render(block->chain, v+0, v+1, v+2);
}

void BR_ASM_CALL RenderForceFront4(struct brp_block *block,
	brp_vertex *v0, brp_vertex *v1,brp_vertex *v2,brp_vertex *v3)
{
	brp_vertex v[4];

	v[0] = *v0;
	v[1] = *v1;
	v[2] = *v2;
	v[3] = *v3;

	v[0].comp[C_SZ] = renderer->state.cache.comp_offsets[C_SZ]-renderer->state.cache.comp_scales[C_SZ];
	v[1].comp[C_SZ] = renderer->state.cache.comp_offsets[C_SZ]-renderer->state.cache.comp_scales[C_SZ];
	v[2].comp[C_SZ] = renderer->state.cache.comp_offsets[C_SZ]-renderer->state.cache.comp_scales[C_SZ];
	v[3].comp[C_SZ] = renderer->state.cache.comp_offsets[C_SZ]-renderer->state.cache.comp_scales[C_SZ];

	block->chain->render(block->chain, v+0, v+1, v+2, v+3);
}

void BR_ASM_CALL RenderForceBack4(struct brp_block *block,
	brp_vertex *v0, brp_vertex *v1,brp_vertex *v2,brp_vertex *v3)
{
	brp_vertex v[4];

	v[0] = *v0;
	v[1] = *v1;
	v[2] = *v2;
	v[3] = *v3;

	v[0].comp[C_SZ] = renderer->state.cache.comp_offsets[C_SZ]+renderer->state.cache.comp_scales[C_SZ];
	v[1].comp[C_SZ] = renderer->state.cache.comp_offsets[C_SZ]+renderer->state.cache.comp_scales[C_SZ];
	v[2].comp[C_SZ] = renderer->state.cache.comp_offsets[C_SZ]+renderer->state.cache.comp_scales[C_SZ];
	v[3].comp[C_SZ] = renderer->state.cache.comp_offsets[C_SZ]+renderer->state.cache.comp_scales[C_SZ];

	block->chain->render(block->chain, v+0, v+1, v+2, v+3);
}

