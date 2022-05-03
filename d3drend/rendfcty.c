/*
 * Copyright (c) 1993-1995 Argonaut Technologies Limited. All rights reserved.
 *
 * $Id: rendfcty.c 1.4 1997/08/11 14:25:59 jon Exp JON $
 * $Locker: JON $
 *
 * Renderer type methods
 */
#include <stddef.h>
#include <string.h>

#include "drv.h"
#include "shortcut.h"
#include "brassert.h"

BR_RCS_ID("$Id: rendfcty.c 1.4 1997/08/11 14:25:59 jon Exp JON $");

/*
 * Default dispatch table for renderer type (defined at and of file)
 */
static const struct br_renderer_facility_dispatch rendererFacilityDispatch;

/*
 * Renderer Type info. template
 */
#define F(f)	offsetof(struct br_renderer_facility, f)

static struct br_tv_template_entry rendererFacilityTemplateEntries[] = {
	{BRT_IDENTIFIER_CSTR,            0,	F(identifier),		BRTV_QUERY | BRTV_ALL,	BRTV_CONV_COPY, },
	{BRT_RENDERER_MAX_I32,           0,	0,					BRTV_QUERY | BRTV_ALL,	BRTV_CONV_DIRECT, 1},
	{BRT(PARTS_TL),                  (br_int_32)&RendererPartsTokens,BRTV_QUERY | BRTV_ALL | BRTV_ABS,	BRTV_CONV_LIST, },
	{BRT(ALTERNATIVE_TO_DEFAULT_B),	 BR_FALSE,       BRTV_QUERY | BRTV_ALL,	BRTV_CONV_DIRECT, },
	{BRT(USES_PRIMITIVE_LIBRARY_B),	 BR_FALSE,       BRTV_QUERY | BRTV_ALL,	BRTV_CONV_DIRECT, },
	{DEV(DIRECT3D_DEVICE_GUID_PTR),  F(d3d.guid),          BRTV_QUERY | BRTV_ALL, BRTV_CONV_COPY, },
	{DEV(DIRECT3D_DEVICE_NAME_CSTR), F(d3d.name),          BRTV_QUERY | BRTV_ALL, BRTV_CONV_COPY, },
	{DEV(DIRECT3D_DEVICE_DESC_CSTR), F(d3d.desc),          BRTV_QUERY | BRTV_ALL, BRTV_CONV_COPY, },
	{DEV(DIRECT3D_HAL_DESC_PTR),     F(d3d.hal_desc),      BRTV_QUERY | BRTV_ALL, BRTV_CONV_COPY, },
	{DEV(DIRECT3D_HEL_DESC_PTR),     F(d3d.hel_desc),      BRTV_QUERY | BRTV_ALL, BRTV_CONV_COPY, },
};
#undef F


struct enumContext {

	br_device *device;
	br_renderer_facility **renderer_facility_list;
	br_uint_32 renderer_facility_list_size;

	struct ddraw_info ddraw;
	LPDIRECT3D2 d3d;

	br_uint_32 renderer_facility_count;
};


static HRESULT FAR PASCAL enumD3DDevice(GUID FAR *guid, LPSTR desc, LPSTR name,
	LPD3DDEVICEDESC hal_desc, LPD3DDEVICEDESC hel_desc, struct enumContext *context);
static br_boolean suitableD3DDevice(LPD3DDEVICEDESC hal_desc, LPD3DDEVICEDESC hel_desc);

/*
 * Build a list of available renderer facilities for a given DirectDraw device
 */
br_error RendererFacilityD3DInitialise(br_device *device, ddraw_info *ddraw,
	br_renderer_facility **renderer_facility_list, br_uint_32 renderer_facility_list_size,
	br_uint_32 *renderer_facility_count)
{
	struct enumContext context;
  
	context.device = device;
	context.renderer_facility_list = renderer_facility_list;
	context.renderer_facility_list_size = renderer_facility_list_size;
	context.ddraw = *ddraw;

	context.renderer_facility_count = 0;

	/*
	 * Get the Direct3D interface for the DirectDraw object
	 */
	if (IDirectDraw2_QueryInterface(context.ddraw.ddraw, &IID_IDirect3D2, &context.d3d) != DD_OK)
		return BRE_FAIL;

	/*
	 * Enumerate all Direct3D devices available for this DirectDraw device
	 */
	IDirect3D2_EnumDevices(context.d3d, enumD3DDevice, &context);

	/*
	 * Release the interface used for enumeration (N.B. New references
	 * will have been added for each renderer facility)
	 */
	IDirect3D2_Release(context.d3d);

	*renderer_facility_count = context.renderer_facility_count;

	return BRE_OK;
}


HRESULT FAR PASCAL enumD3DDevice(GUID FAR *guid, LPSTR desc, LPSTR name,
	LPD3DDEVICEDESC hal_desc, LPD3DDEVICEDESC hel_desc, struct enumContext *context)
{
	br_renderer_facility *renderer_facility;
	struct d3d_info d3d;

	/*
	 * Determine if the device is interesting
	 */
	if (!suitableD3DDevice(hal_desc, hel_desc))
		return DDENUMRET_OK;

	/*
	 * Create a renderer facility for this device
	 */
	d3d.guid = guid;
	d3d.name = name;
	d3d.desc = desc;
#if RENDERER_FACILITY_KEEP_DDRAW
	d3d.ddraw = context->ddraw.ddraw;
#endif
	d3d.d3d = context->d3d;
	d3d.hal_desc = hal_desc;
	d3d.hel_desc = hel_desc;

	renderer_facility = RendererFacilityD3DAllocate(context->device, &context->ddraw, &d3d);
	if (renderer_facility == NULL)
		return DDENUMRET_OK;

	/*
	 * Fill in an entry in the list of output facilities, and stop enumerating if the list is full
	 */
	context->renderer_facility_list[context->renderer_facility_count] = renderer_facility;

	context->renderer_facility_count++;

	if (context->renderer_facility_count == context->renderer_facility_list_size)
		return DDENUMRET_CANCEL;
	
	return DDENUMRET_OK;
}


/*
 * Predicate for whether a given D3D device is suitable
 */
br_boolean suitableD3DDevice(LPD3DDEVICEDESC hal_desc, LPD3DDEVICEDESC hel_desc)
{
	LPD3DDEVICEDESC tri_desc;

	/*
	 * Work out whether we will be using the hal or the hel for each stage
	 */
	if (hal_desc->dwFlags & D3DDD_TRICAPS)
		tri_desc = hal_desc;
	else if (hel_desc->dwFlags & D3DDD_TRICAPS)
		tri_desc = hel_desc;
	else
		return BR_FALSE;

	/*
	 * Check triangle drawing capabilities
	 */
	if ((tri_desc->dwFlags & (D3DDD_DEVCAPS | D3DDD_COLORMODEL)) != (D3DDD_DEVCAPS | D3DDD_COLORMODEL))
		return BR_FALSE;
	
	/*
	 * Check that DrawPrimitive is supported
	 *
	 * N.B. This check doesn't work!
	 */
#if 0
	if (!(tri_desc->dwDevCaps & D3DDEVCAPS_DRAWPRIMTLVERTEX))
		return BR_FALSE;
#endif

	/*
	 * Check that the RGB colour model is supported
	 */
	if (!(tri_desc->dcmColorModel & D3DCOLOR_RGB))
		return BR_FALSE;

	/*
	 * Check that floating point vertex data is acceptable (haven't a clue how we are
	 * meant to pass it if it isn't!)
	 */
	if (!(tri_desc->dwDevCaps & D3DDEVCAPS_FLOATTLVERTEX))
		return BR_FALSE;

	/*
	 * Check that culling can be disabled
	 */
	if (!(tri_desc->dpcTriCaps.dwMiscCaps & D3DPMISCCAPS_CULLNONE))
		return BR_FALSE;

	/*
	 * There are many, many wonderful checks that should be made!
	 */
	//...
	
	return BR_TRUE;
}


/*
 * Set up a renderer type
 */
br_renderer_facility *RendererFacilityD3DAllocate(br_device *device, ddraw_info *ddraw, d3d_info *d3d)
{
    br_renderer_facility *self;
	char temp[512];

	self = BrResAllocate(device, sizeof(*self), BR_MEMORY_OBJECT);

	self->dispatch = (struct br_renderer_facility_dispatch *)&rendererFacilityDispatch;
    self->device = device;
	self->num_instances = 0;
	self->object_list = BrObjectListAllocate(self);

	/*
	 * Make copies of the Direct3D information
	 */
	if (d3d->guid != NULL) {

		self->d3d.guid = BrResAllocate(self, sizeof(*self->d3d.guid), BR_MEMORY_OBJECT_DATA);
		if (self->d3d.guid == NULL) {
			BrResFree(self);
			return NULL;
		}

		BrMemCpy(self->d3d.guid, d3d->guid, sizeof(*self->d3d.guid));

	} else

		self->d3d.guid = NULL;
		
	self->d3d.name = BrResStrDup(self, d3d->name);
	self->d3d.desc = BrResStrDup(self, d3d->desc);

	self->d3d.hal_desc = BrResAllocate(self, sizeof(*self->d3d.hal_desc), BR_MEMORY_OBJECT_DATA);
	self->d3d.hel_desc = BrResAllocate(self, sizeof(*self->d3d.hel_desc), BR_MEMORY_OBJECT_DATA);

	if (self->d3d.name == NULL || self->d3d.desc == NULL ||
		self->d3d.hal_desc == NULL || self->d3d.hel_desc == NULL) {

		BrResFree(self);
		return NULL;
	}
	
	BrMemCpy(self->d3d.hal_desc, d3d->hal_desc, sizeof(*self->d3d.hal_desc));
	BrMemCpy(self->d3d.hel_desc, d3d->hel_desc, sizeof(*self->d3d.hel_desc));

	/*
	 * Make new reference of the Direct3D handle
	 */
#if RENDERER_FACILITY_KEEP_DDRAW
	self->d3d.ddraw = d3d->ddraw;
	IDirect3D2_AddRef(self->d3d.ddraw);
#endif

	self->d3d.d3d = d3d->d3d;
	IDirect3D2_AddRef(self->d3d.d3d);

	/*
	 * Create default state
	 */
	StateInitialise(&self->default_state);
	
	/*
	 * Create geometry objects
	 */
	if (GeometryV1BucketsAllocate(self, "V1Buckets") == NULL ||
		GeometryPrimitivesAllocate(self, "Primitives") == NULL ||
		GeometryLightingAllocate(self, "Lighting") == NULL ||
		GeometryV1ModelAllocate(self, "V1Model") == NULL) {

		IDirect3D2_Release(self->d3d.d3d);
#if RENDERER_FACILITY_KEEP_DDRAW
		IDirectDraw2_Release(self->d3d.ddraw);
#endif
		BrResFree(self);
		return NULL;
	}

	BrSprintf(temp, "%s %s", ddraw->name, self->d3d.name);
		
	self->identifier = BrResStrDup(self, temp);
	if (self->identifier == NULL) {
		IDirect3D2_Release(self->d3d.d3d);
#if RENDERER_FACILITY_KEEP_DDRAW
		IDirectDraw2_Release(self->d3d.ddraw);
#endif
		BrResFree(self);
		return NULL;
	}

	ObjectContainerAddFront(device, (br_object *)self);

	return self;
}

static void BR_CMETHOD_DECL(br_renderer_facility_d3d, free)(br_renderer_facility *self)
{
    /*
     * Detach rendererer from device
     */
	ObjectContainerRemove(self->device, (br_object *) self);

	/*
	 * Remove attached objects
	 */
	BrObjectContainerFree((br_object_container *)self, BR_NULL_TOKEN, NULL, NULL);

	/*
	 * Release the Direct3D interface
	 */
	if (self->d3d.d3d != NULL)
		IDirect3D2_Release(self->d3d.d3d);

#if RENDERER_FACILITY_KEEP_DDRAW
	if (self->d3d.ddraw != NULL)
		IDirectDraw2_Release(self->d3d.ddraw);
#endif

	BrResFreeNoCallback(self);
}

static char *BR_CMETHOD_DECL(br_renderer_facility_d3d, identifier)(br_renderer_facility *self)
{
	return self->identifier;
}

static br_token BR_CMETHOD_DECL(br_renderer_facility_d3d, type)(br_renderer_facility *self)
{
	return BRT_RENDERER_FACILITY;
}

static br_boolean BR_CMETHOD_DECL(br_renderer_facility_d3d, isType)(br_renderer_facility *self, br_token t)
{
	return (t == BRT_RENDERER_FACILITY) || (t == BRT_OBJECT_CONTAINER) || (t == BRT_OBJECT);
}

static struct br_device *BR_CMETHOD_DECL(br_renderer_facility_d3d, device)(br_renderer_facility *self)
{
	return self->device;
}

static br_int_32 BR_CMETHOD_DECL(br_renderer_facility_d3d, space)(br_renderer_facility *self)
{
	return sizeof(br_renderer_facility);
}

static struct br_tv_template * BR_CMETHOD_DECL(br_renderer_facility_d3d, templateQuery)
	(br_renderer_facility *self)
{
    if(self->device->templates.rendererFacilityTemplate == NULL)
        self->device->templates.rendererFacilityTemplate = BrTVTemplateAllocate(self->device,
            (br_tv_template_entry *)rendererFacilityTemplateEntries,
			BR_ASIZE(rendererFacilityTemplateEntries));

    return self->device->templates.rendererFacilityTemplate;
}

static br_error BR_CMETHOD_DECL(br_renderer_facility_d3d, validDestination)(br_renderer *self, br_boolean *bp, br_object *h)
{
	return BRE_OK;
}

/*
 * Structure used to unpack the rendererNew arguments
 */
struct newRendererTokens {
	br_object	*dest;
};

#define F(f)	offsetof(struct newRendererTokens, f)

static struct br_tv_template_entry rendererNewTemplateEntries[] = {
	{BRT_DESTINATION_O,			0,	F(dest),		BRTV_SET,	BRTV_CONV_COPY, },
	{BRT_OUTPUT_FACILITY_O,		0,	F(dest),		BRTV_SET,	BRTV_CONV_COPY, },
};
#undef F

/*
 * Create a new renderer
 */
static br_error BR_CMETHOD_DECL(br_renderer_facility_d3d, rendererNew)
		(struct br_renderer_facility *self, struct br_renderer **prenderer, br_token_value *tv)
{
	struct newRendererTokens rt = { NULL };
	br_int_32 count;
	br_renderer *renderer;

	/*
	 * Process any options
	 */
    if (self->device->templates.rendererNewTemplate == NULL)
       self->device->templates.rendererNewTemplate = BrTVTemplateAllocate(self->device,
            rendererNewTemplateEntries, BR_ASIZE(rendererNewTemplateEntries));

	BrTokenValueSetMany(&rt, &count, NULL, tv, self->device->templates.rendererNewTemplate);

	/*
	 * Check that the destination is valid
	 */
	if (rt.dest == NULL || ObjectDevice(rt.dest) != self->device || !ObjectIsType(rt.dest, BRT_DEVICE_PIXELMAP))

		return BRE_UNSUPPORTED;

	/*
	 * Create Renderer
	 */
	renderer = RendererD3DAllocate(self->device, self, (br_device_pixelmap *)rt.dest);

	if(renderer == NULL)
		return BRE_FAIL;

	self->num_instances++;

	*prenderer	= renderer;
	return BRE_OK;
}

static void * BR_CMETHOD_DECL(br_renderer_facility_d3d,listQuery)(br_device *self)
{
	return self->object_list;
}

/*
 * Default dispatch table for renderer type (defined at and of file)
 */
static const struct br_renderer_facility_dispatch rendererFacilityDispatch = {
	NULL,
	NULL,
	NULL,
	NULL,
	BR_CMETHOD_REF(br_renderer_facility_d3d,	free),
	BR_CMETHOD_REF(br_renderer_facility_d3d,	identifier),
	BR_CMETHOD_REF(br_renderer_facility_d3d,	type),
	BR_CMETHOD_REF(br_renderer_facility_d3d,	isType),
	BR_CMETHOD_REF(br_renderer_facility_d3d,	device),
	BR_CMETHOD_REF(br_renderer_facility_d3d,	space),

	BR_CMETHOD_REF(br_renderer_facility_d3d,	templateQuery),
	BR_CMETHOD_REF(br_object,					query),
	BR_CMETHOD_REF(br_object,					queryBuffer),
	BR_CMETHOD_REF(br_object,					queryMany),
	BR_CMETHOD_REF(br_object,					queryManySize),
	BR_CMETHOD_REF(br_object,					queryAll),
	BR_CMETHOD_REF(br_object,					queryAllSize),

	BR_CMETHOD_REF(br_renderer_facility_d3d,	listQuery),
	BR_CMETHOD_REF(br_object_container,			tokensMatchBegin),
	BR_CMETHOD_REF(br_object_container,			tokensMatch),
	BR_CMETHOD_REF(br_object_container,			tokensMatchEnd),
	BR_CMETHOD_REF(br_object_container,			addFront),
	BR_CMETHOD_REF(br_object_container,			removeFront),
	BR_CMETHOD_REF(br_object_container,			remove),
	BR_CMETHOD_REF(br_object_container,			find),
	BR_CMETHOD_REF(br_object_container,			findMany),
	BR_CMETHOD_REF(br_object_container,			count),

	BR_CMETHOD_REF(br_renderer_facility_d3d,	validDestination),

	BR_CMETHOD_REF(br_renderer_facility_d3d,	rendererNew),
};

