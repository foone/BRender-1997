/*
 * Copyright (c) 1993-1995 Argonaut Technologies Limited. All rights reserved.
 *
 * $Id: gv1model.c 1.1 1997/07/11 16:14:09 jon Exp JON $
 * $Locker: JON $
 *
 * Geometry format for version 1 models
 */
#include <stddef.h>
#include <string.h>

#include "drv.h"
#include "shortcut.h"
#include "brassert.h"

BR_RCS_ID("$Id: gv1model.c 1.1 1997/07/11 16:14:09 jon Exp JON $");

/*
 * Default dispatch table for renderer type (defined at and of file)
 */
static const struct br_geometry_v1_model_dispatch geometryV1ModelDispatch;

/*
 * Geometry format info. template
 */
#define F(f)	offsetof(struct br_geometry_v1_model, f)

static const struct br_tv_template_entry geometryV1ModelTemplateEntries[] = {
	{BRT_IDENTIFIER_CSTR,	0,	F(identifier),		BRTV_QUERY | BRTV_ALL,	BRTV_CONV_COPY, },
	{BRT_RENDERER_FACILITY_O,0,	F(renderer_facility),	BRTV_QUERY | BRTV_ALL,	BRTV_CONV_COPY, },
	{BRT_FACILITY_O,			0,	F(renderer_facility),	BRTV_QUERY,				BRTV_CONV_COPY, },
};
#undef F

/*
 * Allocate a geometry format
 */
br_geometry_v1_model * GeometryV1ModelAllocate(br_renderer_facility *type, char *id)
{
	br_geometry_v1_model * self;

	self = BrResAllocate(type, sizeof(*self), BR_MEMORY_OBJECT);

	if(self == NULL)
		return NULL;

	self->dispatch = (struct br_geometry_v1_model_dispatch *)&geometryV1ModelDispatch;
 	self->identifier = id;
    self->device = type->device;
 	self->renderer_facility = type;

	ObjectContainerAddFront(type, (br_object *)self);
	return self;
}

static void BR_CMETHOD_DECL(br_geometry_v1_model_d3d, free)(br_geometry_v1_model *self)
{
	ObjectContainerRemove(self->renderer_facility, (br_object *)self);

	BrResFreeNoCallback(self);
}

static br_token BR_CMETHOD_DECL(br_geometry_v1_model_d3d, type)(br_geometry_v1_model *self)
{
	return BRT_GEOMETRY_V1_MODEL;
}

static br_boolean BR_CMETHOD_DECL(br_geometry_v1_model_d3d, isType)(br_geometry_v1_model *self, br_token t)
{
	return (t == BRT_GEOMETRY_V1_MODEL) || (t == BRT_GEOMETRY) || (t == BRT_OBJECT);
}

static br_int_32 BR_CMETHOD_DECL(br_geometry_v1_model_d3d, space)(br_geometry_v1_model *self)
{
	return sizeof(br_geometry_v1_model);
}

static struct br_tv_template * BR_CMETHOD_DECL(br_geometry_v1_model_d3d, templateQuery)
	(br_geometry_v1_model *self)
{
    if(self->device->templates.geometryV1ModelTemplate == NULL)
        self->device->templates.geometryV1ModelTemplate = BrTVTemplateAllocate(self->device,
            (br_tv_template_entry *)geometryV1ModelTemplateEntries,
			BR_ASIZE(geometryV1ModelTemplateEntries));

    return self->device->templates.geometryV1ModelTemplate;
}

static br_error BR_CMETHOD_DECL(br_geometry_v1_model_d3d, storedAvail)(
		struct br_geometry *self,
		br_int_32 *psize,
		br_token_value *tv)
{
	return BRE_FAIL;
}

#if BASED_FIXED
static br_error BR_CMETHOD_DECL(br_geometry_v1_model_d3d, storedNewFloatToFixed)
		(struct br_geometry *self, struct br_renderer *renderer,
		struct br_geometry_stored **psg, struct v11model_f *model, br_token type, br_token_value *tv)
{
	return BRE_FAIL;
}
#endif

#if BASED_FLOAT
static br_error BR_CMETHOD_DECL(br_geometry_v1_model_d3d, storedNewFixedToFloat)
		(struct br_geometry *self, struct br_renderer *renderer,
		struct br_geometry_stored **psg, struct v11model_f *model, br_token type, br_token_value *tv)
{
	return BRE_FAIL;
}
#endif

static br_error BR_CMETHOD_DECL(br_geometry_v1_model_d3d, storedNew)
		(struct br_geometry *self, struct br_renderer *renderer,
		struct br_geometry_stored **psg, struct v11model *model, br_token type, br_token_value *tv)
{
	return BRE_FAIL;
}

#if BASED_FIXED
static br_error BR_CMETHOD_DECL(br_geometry_v1_model_d3d, renderFloatToFixed)
		(struct br_geometry *self, struct br_renderer *renderer, struct v11model *model, br_token type)
{
	return BRE_FAIL;
}

static br_error BR_CMETHOD_DECL(br_geometry_v1_model_d3d, renderOnScreenFloatToFixed)
		(struct br_geometry *self, struct br_renderer *renderer, struct v11model *model, br_token type)
{
	return BRE_FAIL;
}
#endif

#if BASED_FLOAT
static br_error BR_CMETHOD_DECL(br_geometry_v1_model_d3d, renderFixedToFloat)
		(struct br_geometry *self, struct br_renderer *renderer, struct v11model *model, br_token type)
{
	return BRE_FAIL;
}

static br_error BR_CMETHOD_DECL(br_geometry_v1_model_d3d, renderOnScreenFixedToFloat)
		(struct br_geometry *self, struct br_renderer *renderer, struct v11model *model, br_token type)
{
	return BRE_FAIL;
}
#endif

/*
 * Default dispatch table for renderer type
 */
static const struct br_geometry_v1_model_dispatch geometryV1ModelDispatch = {
	NULL,
	NULL,
	NULL,
	NULL,
	BR_CMETHOD_REF(br_geometry_v1_model_d3d,	free),
	BR_CMETHOD_REF(br_object_d3d,				identifier),
	BR_CMETHOD_REF(br_geometry_v1_model_d3d,	type),
	BR_CMETHOD_REF(br_geometry_v1_model_d3d,	isType),
	BR_CMETHOD_REF(br_object_d3d,				device),
	BR_CMETHOD_REF(br_geometry_v1_model_d3d,	space),

	BR_CMETHOD_REF(br_geometry_v1_model_d3d,	templateQuery),
	BR_CMETHOD_REF(br_object,					query),
	BR_CMETHOD_REF(br_object,					queryBuffer),
	BR_CMETHOD_REF(br_object,					queryMany),
	BR_CMETHOD_REF(br_object,					queryManySize),
	BR_CMETHOD_REF(br_object,					queryAll),
	BR_CMETHOD_REF(br_object,					queryAllSize),

#if BASED_FIXED
	BR_CMETHOD_REF(br_geometry_v1_model_d3d,	renderFloatToFixed),
	BR_CMETHOD_REF(br_geometry_v1_model_d3d,	render),
	BR_CMETHOD_REF(br_geometry_v1_model_d3d,	renderOnScreenFloatToFixed),
	BR_CMETHOD_REF(br_geometry_v1_model_d3d,	renderOnScreen),
	BR_CMETHOD_REF(br_geometry_v1_model_d3d,	storedNewFloatToFixed),
	BR_CMETHOD_REF(br_geometry_v1_model_d3d,	storedNew),
#endif

#if BASED_FLOAT
	BR_CMETHOD_REF(br_geometry_v1_model_d3d,	render),
	BR_CMETHOD_REF(br_geometry_v1_model_d3d,	renderFixedToFloat),
	BR_CMETHOD_REF(br_geometry_v1_model_d3d,	renderOnScreen),
	BR_CMETHOD_REF(br_geometry_v1_model_d3d,	renderOnScreenFixedToFloat),
	BR_CMETHOD_REF(br_geometry_v1_model_d3d,	storedNew),
	BR_CMETHOD_REF(br_geometry_v1_model_d3d,	storedNewFixedToFloat),
#endif

	BR_CMETHOD_REF(br_geometry_v1_model_d3d,	storedAvail),
};

