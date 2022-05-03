/*
 * Copyright (c) 1993-1995 Argonaut Technologies Limited. All rights reserved.
 *
 * $Id: sstate.c 1.2 1997/08/11 14:26:07 jon Exp JON $
 * $Locker: JON $
 *
 * Stored renderer state 
 */
#include <stddef.h>
#include <string.h>

#include "drv.h"
#include "shortcut.h"
#include "brassert.h"

BR_RCS_ID("$Id: sstate.c 1.2 1997/08/11 14:26:07 jon Exp JON $");

/*
 * Default dispatch table for renderer type (defined at and of file)
 */
static const struct br_renderer_state_stored_dispatch rendererStateStoredDispatch;

/*
 * Geometry format info. template
 */
#define F(f)	offsetof(struct br_renderer_state_stored, f)

static struct br_tv_template_entry rendererStateStoredTemplateEntries[] = {
	{BRT_IDENTIFIER_CSTR,	0,	F(identifier),		BRTV_QUERY | BRTV_ALL,	BRTV_CONV_COPY, },
	{BRT_RENDERER_O,		0,	F(renderer),	BRTV_QUERY | BRTV_ALL,	BRTV_CONV_COPY, },
	{BRT_PARTS_U32,			0,	F(valid),		BRTV_QUERY | BRTV_ALL,	BRTV_CONV_COPY, },
};
#undef F

/*
 * Utility functions to copy all or part of a state from one place to another
 * where either source or destination is a stored state
 *
 * The destination recalc. bitmasks are updated appropiately
 */
br_error StateCopyToStored(struct br_renderer_state_stored *dest, struct state_all *src, br_uint_32 copy_mask, void *res)
{
#if 1
	if(copy_mask & (MASK_STATE_SURFACE | MASK_STATE_CULL))
		copy_mask |= MASK_STATE_CACHE;
#endif

	/*
	 * Restrict copy_mask to parts held in stored states (should it return
	 * an error if valid parts will be lost?)
	 */
	copy_mask &= MASK_STATE_STORED;

	/*
	 * Restrict copy_mask to valid source parts
	 */
	copy_mask &= src->valid;

	/*
	 * Merge valid mask into destination
	 */
	dest->valid |= copy_mask;

	if(copy_mask & MASK_STATE_CULL)
		dest->cull = src->cull;

	if(copy_mask & MASK_STATE_SURFACE)
		dest->surface = src->surface;

	if(copy_mask & MASK_STATE_PRIMITIVE)
		dest->prim = src->prim;

	if(copy_mask & MASK_STATE_CACHE)
		dest->cache = src->cache;

#if 1
	/*
	 * Mark destination's cache as invalid if necessary
	 */
	if((copy_mask & (MASK_STATE_SURFACE | MASK_STATE_CULL)) != (MASK_STATE_SURFACE | MASK_STATE_CULL))
		dest->cache.valid = BR_FALSE;
#endif

	return BRE_OK;
}

br_error StateCopyFromStored(struct state_all *dest, struct br_renderer_state_stored *src, br_uint_32 copy_mask, void *res)
{
#if 1
	if(copy_mask & (MASK_STATE_SURFACE | MASK_STATE_CULL))
		copy_mask |= MASK_STATE_CACHE;
#endif

	/*
	 * Restrict copy_mask to parts held in stored states (should it return
	 * an error if requested parts will not be copied?)
	 */
	copy_mask &= MASK_STATE_STORED;

	/*
	 * Restrict copy_mask to valid source parts
	 */
	copy_mask &= src->valid;

	/*
	 * Merge valid mask into destination
	 */
	dest->valid |= copy_mask;

	if(copy_mask & MASK_STATE_CULL)
		dest->cull = src->cull;

	if(copy_mask & MASK_STATE_SURFACE)
		dest->surface = src->surface;

	if(copy_mask & MASK_STATE_PRIMITIVE)
		dest->prim = src->prim;

	if(copy_mask & MASK_STATE_CACHE)
		dest->cache = src->cache;

#if 1
	/*
	 * Mark destination's cache as invalid if necessary
	 */
	if((copy_mask & (MASK_STATE_SURFACE | MASK_STATE_CULL)) != (MASK_STATE_SURFACE | MASK_STATE_CULL))
		dest->cache.valid = BR_FALSE;
#endif

	return BRE_OK;
}

/*
 * Allocate a stored state
 */
br_renderer_state_stored *RendererStateStoredD3DAllocate(br_renderer *renderer,
	struct state_all *base_state, br_uint_32 m, br_token_value *tv)
{
	br_renderer_state_stored * self;

	self = BrResAllocate(renderer, sizeof(*self), BR_MEMORY_OBJECT);

	if(self == NULL)
		return NULL;

	self->dispatch = (struct br_renderer_state_stored_dispatch *)&rendererStateStoredDispatch;
 	self->identifier = "Renderer-State-Stored";
    self->device = renderer->device;
 	self->renderer = renderer;

	/*
	 * Copy initial state
	 */
	StateCopyToStored(self, base_state, m, self);

	ObjectContainerAddFront(renderer, (br_object *)self);

	return self;
}

static void BR_CMETHOD_DECL(br_renderer_state_stored_d3d, free)(br_renderer_state_stored *self)
{
	ObjectContainerRemove(self->renderer, (br_object *)self);

	/*
	 * Any associated primitive state will have been attached as a resource
	 */
	BrResFreeNoCallback(self);
}

static br_token BR_CMETHOD_DECL(br_renderer_state_stored_d3d, type)(br_renderer_state_stored *self)
{
	return BRT_RENDERER_STATE_STORED;
}

static br_boolean BR_CMETHOD_DECL(br_renderer_state_stored_d3d, isType)(br_renderer_state_stored *self, br_token t)
{
	return (t == BRT_RENDERER_STATE_STORED) || (t == BRT_OBJECT);
}

static br_int_32 BR_CMETHOD_DECL(br_renderer_state_stored_d3d, space)(br_renderer_state_stored *self)
{
	return sizeof(br_renderer_state_stored);
}

static struct br_tv_template * BR_CMETHOD_DECL(br_renderer_state_stored_d3d, templateQuery)
	(br_renderer_state_stored *self)
{
    if(self->device->templates.rendererStateStoredTemplate == NULL)
        self->device->templates.rendererStateStoredTemplate = BrTVTemplateAllocate(self->device,
            (br_tv_template_entry *)rendererStateStoredTemplateEntries,
			BR_ASIZE(rendererStateStoredTemplateEntries));

    return self->device->templates.rendererStateStoredTemplate;
}

/*
 * Default dispatch table for renderer type (defined at and of file)
 */
static const struct br_renderer_state_stored_dispatch rendererStateStoredDispatch = {
	NULL,
	NULL,
	NULL,
	NULL,
	BR_CMETHOD_REF(br_renderer_state_stored_d3d,		free),
	BR_CMETHOD_REF(br_object_d3d,						identifier),
	BR_CMETHOD_REF(br_renderer_state_stored_d3d,		type),
	BR_CMETHOD_REF(br_renderer_state_stored_d3d,		isType),
	BR_CMETHOD_REF(br_object_d3d,						device),
	BR_CMETHOD_REF(br_renderer_state_stored_d3d,		space),

	BR_CMETHOD_REF(br_renderer_state_stored_d3d,		templateQuery),
	BR_CMETHOD_REF(br_object,							query),
	BR_CMETHOD_REF(br_object,							queryBuffer),
	BR_CMETHOD_REF(br_object,							queryMany),
	BR_CMETHOD_REF(br_object,							queryManySize),
	BR_CMETHOD_REF(br_object,							queryAll),
	BR_CMETHOD_REF(br_object,							queryAllSize),
};

