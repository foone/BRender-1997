/*
 * Copyright (c) 1993-1995 Argonaut Technologies Limited. All rights reserved.
 *
 * $Id: Outfcty.c 1.4 1997/08/11 14:25:14 jon Exp JON $
 * $Locker: JON $
 *
 * Output type methods
 */
#include <stddef.h>
#include <string.h>

#include "drv.h"
#include "host.h"
#include "shortcut.h"
#include "brassert.h"

BR_RCS_ID("$Id: Outfcty.c 1.4 1997/08/11 14:25:14 jon Exp JON $");


/*
 * Default dispatch table for device (defined at end of file)
 */
static const struct br_output_facility_dispatch outputFacilityDispatch;

/*
 * Output Type info. template
 */
#define F(f)	offsetof(struct br_output_facility, f)

static struct br_tv_template_entry outputFacilityTemplateEntries[] = {

	{BRT(WIDTH_I32),                  F(width),                  BRTV_QUERY | BRTV_ALL, BRTV_CONV_COPY, },
	{BRT(WIDTH_MIN_I32),              F(min_width),              BRTV_QUERY | BRTV_ALL, BRTV_CONV_COPY, },
	{BRT(WIDTH_MAX_I32),              F(max_width),              BRTV_QUERY | BRTV_ALL, BRTV_CONV_COPY, },
	{BRT(HEIGHT_I32),                 F(height),                 BRTV_QUERY | BRTV_ALL, BRTV_CONV_COPY, },
	{BRT(HEIGHT_MIN_I32),             F(min_height),             BRTV_QUERY | BRTV_ALL, BRTV_CONV_COPY, },
	{BRT(HEIGHT_MAX_I32),             F(max_height),             BRTV_QUERY | BRTV_ALL, BRTV_CONV_COPY, },
	{BRT(PIXEL_BITS_I32),             F(colour_bits),            BRTV_QUERY | BRTV_ALL, BRTV_CONV_COPY, },
	{BRT(PIXEL_TYPE_U8),              F(colour_type),            BRTV_QUERY | BRTV_ALL, BRTV_CONV_COPY, },
	{BRT(INDEXED_B),                  F(indexed),                BRTV_QUERY | BRTV_ALL, BRTV_CONV_COPY, },
	{BRT(WINDOW_FULLSCREEN_B),        F(fullscreen),             BRTV_QUERY | BRTV_ALL, BRTV_CONV_COPY, },
	{DEV(REFRESH_RATE_U32),           F(refresh_rate),           BRTV_QUERY | BRTV_ALL, BRTV_CONV_COPY, },
	{BRT(IDENTIFIER_CSTR),            F(identifier),             BRTV_QUERY | BRTV_ALL, BRTV_CONV_COPY, },
	{BRT(RENDERER_FACILITY_O),        F(renderer_facility),      BRTV_QUERY | BRTV_ALL, BRTV_CONV_COPY, },
	{BRT(RENDERER_FACILITY_OL),       F(renderer_facility_list), BRTV_QUERY | BRTV_ALL, BRTV_CONV_COPY, },
	{BRT(WINDOW_HANDLE_H),            (br_uint_32)NULL,          BRTV_QUERY | BRTV_ALL,	BRTV_CONV_DIRECT, },
	{DEV(DIRECTDRAW_DEVICE_GUID_PTR), F(ddraw.guid),             BRTV_QUERY | BRTV_ALL, BRTV_CONV_COPY, },
	{DEV(DIRECTDRAW_DEVICE_NAME_CSTR),F(ddraw.name),             BRTV_QUERY | BRTV_ALL, BRTV_CONV_COPY, },
	{DEV(DIRECTDRAW_DEVICE_DESC_CSTR),F(ddraw.desc),             BRTV_QUERY | BRTV_ALL, BRTV_CONV_COPY, },
	{DEV(DIRECTDRAW_HAL_CAPS_PTR),    F(ddraw.hal_caps),         BRTV_QUERY | BRTV_ALL, BRTV_CONV_COPY, },
	{DEV(DIRECTDRAW_HEL_CAPS_PTR),    F(ddraw.hel_caps),         BRTV_QUERY | BRTV_ALL, BRTV_CONV_COPY, },
	{DEV(DIRECTDRAW_SURFACE_DESC_PTR),F(ddraw.surface_desc),     BRTV_QUERY | BRTV_ALL, BRTV_CONV_COPY, },
};

#undef F


struct enumContext {

	br_device *device;
	br_renderer_facility **renderer_facility_list;
	br_uint_32 renderer_facility_count;

	struct ddraw_info ddraw;

	br_uint_32 output_facility_count;
};


static HRESULT FAR PASCAL enumMode(LPDDSURFACEDESC desc, struct enumContext *context);
static br_boolean suitableMode(LPDDSURFACEDESC surface_desc);


/*
 * Build a list of available output facilities for a given DirectDraw device
 */
br_error OutputFacilityD3DInitialise(br_device *device, ddraw_info *ddraw,
	br_renderer_facility **renderer_facility_list, br_uint_32 renderer_facility_count,
	br_uint_32 *output_facility_count)
{
	struct enumContext context;
	DDSURFACEDESC surface_desc = { sizeof(DDSURFACEDESC) };
  
	context.device = device;
	context.renderer_facility_list = renderer_facility_list;
	context.renderer_facility_count = renderer_facility_count;

	context.ddraw = *ddraw;

	context.output_facility_count = 0;

	/*
	 * Get the current display mode
	 *
	 * *** N.B. Not sure how to detect if windows are available on this device (for
	 * Win95 only the display device can have windows, but Memphis allows multi-monitor
	 * displays) ***
	 */
#if 1
	/*
	 * This is duff - windows can only be on primary device
	 */
	if (context.ddraw.guid == NULL && IDirectDraw2_GetDisplayMode(context.ddraw.ddraw, &surface_desc) == DD_OK)
#else
#if 1
	/*
	 * Try to set a normal cooperative level to determine if windowed operation is available
	 *
	 * Doesn't work - 3Dfx happily allows it.
	 */
	if (IDirectDraw2_SetCooperativeLevel(context.ddraw.ddraw, NULL, DDSCL_NORMAL) == DD_OK &&
		IDirectDraw2_GetDisplayMode(context.ddraw.ddraw, &surface_desc) == DD_OK)

#else
	/*
	 * This doesn't appear to work (DDCAPS_GDI flag never set that I have seen)
	 */
	if (context.ddraw.hal_caps->dwCaps & DDCAPS_GDI &&
		IDirectDraw2_GetDisplayMode(context.ddraw.ddraw, &surface_desc) == DD_OK)
#endif
#endif

		/*
		 * Determine if the mode is interesting
		 */
		 if (suitableMode(&surface_desc)) {

			context.ddraw.surface_desc = &surface_desc;

			/*
			 * Create an output facility for the current display mode
			 */
			if (OutputFacilityD3DAllocate(context.device, context.renderer_facility_list, context.renderer_facility_count,
				&context.ddraw, BR_FALSE) != NULL)

				context.output_facility_count++;
		 }

	/*
	 * Look for overlay support
	 */
	//...

	/*
	 * Enumerate all display modes available for this DirectDraw device
	 */
	IDirectDraw2_EnumDisplayModes(context.ddraw.ddraw, DDEDM_REFRESHRATES, NULL, &context, enumMode);
	
	*output_facility_count = context.output_facility_count;

	return BRE_OK;
}


HRESULT FAR PASCAL enumMode(LPDDSURFACEDESC surface_desc, struct enumContext *context)
{
	/*
	 * Determine if the mode is interesting
	 */
	if (!suitableMode(surface_desc))
		return DDENUMRET_OK;

	/*
	 * Create an output facility for this mode
	 */
	context->ddraw.surface_desc = surface_desc;
	
	if (OutputFacilityD3DAllocate(context->device, context->renderer_facility_list, context->renderer_facility_count,
		&context->ddraw, BR_TRUE) != NULL)

		context->output_facility_count++;

	return DDENUMRET_OK;
}


br_boolean suitableMode(LPDDSURFACEDESC surface_desc)
{
	return BR_TRUE;
}


static br_boolean suitableD3DDeviceForMode(LPD3DDEVICEDESC hal_desc, LPD3DDEVICEDESC hel_desc, LPDDSURFACEDESC surface_desc);
static br_boolean betterD3DDeviceForMode(LPD3DDEVICEDESC hal_desc1, LPD3DDEVICEDESC hel_desc1, LPD3DDEVICEDESC hal_desc2, LPD3DDEVICEDESC hel_desc2, LPDDSURFACEDESC surface_desc);


br_output_facility *OutputFacilityD3DAllocate(br_device *device,
	br_renderer_facility **renderer_facility_list, br_uint_32 renderer_facility_count,
	struct ddraw_info *ddraw, br_boolean full_screen)
{
	br_output_facility *self;
	br_renderer_facility **valid_facility;
	br_uint_32 num_valid_facilities;
	char temp[512];
	br_uint_32 i, j;

	/*
	 * Allocate an object and fill it in
	 */
	self = BrResAllocate(device, sizeof(*self), BR_MEMORY_OBJECT);

	self->dispatch = (struct br_output_facility_dispatch *)&outputFacilityDispatch;

	self->device = device;

	if (DDPixelFormatToPixelType(&self->colour_type, &ddraw->surface_desc->ddpfPixelFormat) != BRE_OK) {
		BrResFree(self);
		return NULL;
	}
		
	self->colour_bits = PixelTypePixelBits(self->colour_type);
	self->indexed = PixelTypeIndexed(self->colour_type);

	self->width = ddraw->surface_desc->dwWidth;
	self->height = ddraw->surface_desc->dwHeight;

	if (full_screen) {
		self->min_width = ddraw->surface_desc->dwWidth;
		self->min_height = ddraw->surface_desc->dwHeight;
	} else {
		self->min_width = 1;
		self->min_height = 1;
	}

	self->max_width = ddraw->surface_desc->dwWidth;
	self->max_height = ddraw->surface_desc->dwHeight;

	self->fullscreen = full_screen;

	self->refresh_rate = ddraw->surface_desc->dwRefreshRate;

	/*
	 * Make a list of the renderer facilities that are suitable for this mode
	 *
	 * N.B. I am fairly liberal here in case someone intends to render to a different format
	 * back buffer
	 */
	valid_facility = BrScratchAllocate(renderer_facility_count * sizeof(*renderer_facility_list));
	if (valid_facility == NULL) {
		BrResFree(self);
		return NULL;
	}

	num_valid_facilities = 0;

	for (i = 0; i < renderer_facility_count; i++)

		if (suitableD3DDeviceForMode(renderer_facility_list[i]->d3d.hal_desc, renderer_facility_list[i]->d3d.hel_desc, ddraw->surface_desc)) {

			valid_facility[num_valid_facilities] = renderer_facility_list[i];
			num_valid_facilities++;
		}

	if (num_valid_facilities == 0) {
		BrResFree(self);
		return NULL;
	}
		
	/*
	 * Sort the valid renderer facilities according to suitability
	 */
	for (i = 0; i < num_valid_facilities; i++)
		for (j = num_valid_facilities - 1; j > i; j--)

			if (betterD3DDeviceForMode(valid_facility[j]->d3d.hal_desc, valid_facility[j]->d3d.hel_desc,
					valid_facility[i]->d3d.hal_desc, valid_facility[i]->d3d.hel_desc,
					ddraw->surface_desc)) {

				br_renderer_facility *temp = valid_facility[i];
				valid_facility[i] = valid_facility[j];
				valid_facility[j] = temp;
			}

	/*
	 * Make a NULL-terminated copy of this sorted list
	 */
	self->renderer_facility_list = BrResAllocate(self, (num_valid_facilities + 1) * sizeof(*renderer_facility_list), BR_MEMORY_OBJECT_DATA);
	if (self->renderer_facility_list == NULL) {
		BrResFree(self);
		return NULL;
	}

	BrMemCpy(self->renderer_facility_list, valid_facility, num_valid_facilities * sizeof(*renderer_facility_list));
	self->renderer_facility_list[num_valid_facilities] = NULL;
	
	BrScratchFree(valid_facility);
	
	/*
	 * Set the default renderer facility to be the best one
	 */
	self->renderer_facility = self->renderer_facility_list[0];

	/*
	 * Copy the DirectDraw information
	 */
	self->ddraw.guid = ddraw->guid;
	self->ddraw.name = ddraw->name;
	self->ddraw.desc = ddraw->desc;
	self->ddraw.hal_caps = ddraw->hal_caps;
	self->ddraw.hel_caps = ddraw->hel_caps;

	/*
	 * Make a copy of the surface description
	 */
	self->ddraw.surface_desc = BrResAllocate(self, sizeof(*self->ddraw.surface_desc), BR_MEMORY_OBJECT_DATA);
	if (self->ddraw.surface_desc == NULL) {
		BrResFree(self);
		return NULL;
	}
	
	BrMemCpy(self->ddraw.surface_desc, ddraw->surface_desc, sizeof(*self->ddraw.surface_desc));

	/*
	 * Make new reference of the DirectDraw and Direct3D handles
	 */
	self->ddraw.ddraw = ddraw->ddraw;
	IDirectDraw2_AddRef(self->ddraw.ddraw);

	if (full_screen)
		BrSprintf(temp, "%s %dx%dx%d", self->ddraw.name, self->width, self->height, self->colour_bits);
	else
		BrSprintf(temp, "%s windowed", self->ddraw.name);
		
	self->identifier = BrResStrDup(self, temp);
	if (self->identifier == NULL) {
		IDirectDraw2_Release(self->ddraw.ddraw);
		BrResFree(self);
		return NULL;
	}

	self->object_list = BrObjectListAllocate(self);
	if (self->object_list == NULL) {
		IDirectDraw2_Release(self->ddraw.ddraw);
		BrResFree(self);
		return NULL;
	}

	ObjectContainerAddFront(device, (br_object *)self);

	return self;
}


/*
 * Predicate for whether a given D3D device is suitable for rendering to a given mode
 */
static br_boolean suitableD3DDeviceForMode(LPD3DDEVICEDESC hal_desc, LPD3DDEVICEDESC hel_desc, LPDDSURFACEDESC surface_desc)
{
	/*
	 * N.B. May not be rendering directly to primary buffer, so cannot reject on the grounds
	 * of inability to render to front buffer
	 */
	return BR_TRUE;
}


static br_boolean betterD3DDeviceForMode(LPD3DDEVICEDESC hal_desc1, LPD3DDEVICEDESC hel_desc1, LPD3DDEVICEDESC hal_desc2, LPD3DDEVICEDESC hel_desc2, LPDDSURFACEDESC surface_desc)
{
	LPD3DDEVICEDESC tri_desc1, tri_desc2;

	/*
	 * Work out whether we will be using the hal or the hel for each stage
	 * Having HAL functionality is always better
	 */
	if (hal_desc1->dwFlags & D3DDD_TRICAPS) {

		tri_desc1 = hal_desc1;

		if (hal_desc2->dwFlags & D3DDD_TRICAPS)
			tri_desc2 = hal_desc2;
		else
			return BR_TRUE;

	} else {

		tri_desc1 = hel_desc1;

		if (hal_desc2->dwFlags & D3DDD_TRICAPS)
			return BR_FALSE;
		else
			tri_desc2 = hel_desc2;
	}

	/*
	 * MMX is better than RGB
	 */
	//...

	/*
	 * Otherwise, there is nothing to choose between them
	 */
	return BR_FALSE;
}

/*
 * Common object methods
 */
static void BR_CMETHOD_DECL(br_output_facility_d3d, free)(br_output_facility *self)
{
	ObjectContainerRemove(self->device, (br_object *)self);

	/*
	 * Remove attached objects
	 */
	BrObjectContainerFree((br_object_container *)self, BR_NULL_TOKEN, NULL, NULL);

	/*
	 * Release the DirectDraw interface
	 */
	if (self->ddraw.ddraw != NULL)
		IDirectDraw2_Release(self->ddraw.ddraw);

	BrResFreeNoCallback(self);
}

static char *BR_CMETHOD_DECL(br_output_facility_d3d, identifier)(br_output_facility *self)
{
	return self->identifier;
}

static br_token BR_CMETHOD_DECL(br_output_facility_d3d, type)(br_output_facility *self)
{
	return BRT_OUTPUT_FACILITY;
}

static br_boolean BR_CMETHOD_DECL(br_output_facility_d3d, isType)(br_output_facility *self, br_token t)
{
	return (t == BRT_OUTPUT_FACILITY) || (t == BRT_OBJECT_CONTAINER) || (t == BRT_OBJECT);
}

static br_device *BR_CMETHOD_DECL(br_output_facility_d3d, device)(br_output_facility *self)
{
	return self->device;
}

static br_int_32 BR_CMETHOD_DECL(br_output_facility_d3d, space)(br_output_facility *self)
{
	return sizeof(br_output_facility);
}

static struct br_tv_template * BR_CMETHOD_DECL(br_output_facility_d3d, queryTemplate)(br_output_facility *self)
{
    if(self->device->templates.outputFacilityTemplate== NULL)
        self->device->templates.outputFacilityTemplate= BrTVTemplateAllocate(self->device,
            outputFacilityTemplateEntries,
            BR_ASIZE(outputFacilityTemplateEntries));

    return self->device->templates.outputFacilityTemplate;
}

static br_error BR_CMETHOD_DECL(br_output_facility_d3d, validSource)(br_output_facility *self, br_boolean *bp, br_object *h)
{
	return BRE_OK;
}

/*
 * br_output_facility_d3d::pixelmapNew
 *
 * Instantiate an output pixelmap from the output type
 */
static br_error BR_CMETHOD_DECL(br_output_facility_d3d, pixelmapNew)
	(br_output_facility *self, br_device_pixelmap **ppmap, br_token_value *tv)
{
	br_device_pixelmap *pm;

	/*
	 * Create a device pixelmap structure representing display memory
	 */
	pm = DevicePixelmapD3DAllocate(self->device, self, tv);

	if(pm == NULL)
		return BRE_FAIL;

	*ppmap = pm;

	return BRE_OK;
}

/*
 * Cannot create new CLUTs, stuck with the single hardware one
 */
static br_error BR_CMETHOD_DECL(br_output_facility_d3d, clutNew)(br_output_facility *self,
	br_device_clut **pclut, br_token_value *tv)
{
	return BRE_FAIL;
}

static br_error BR_CMETHOD_DECL(br_output_facility_d3d, queryCapability)(
    br_token_value *buffer_in, br_token_value *buffer_out, br_size_t size_buffer_out)
{
	return BRE_FAIL;
}



static void * BR_CMETHOD_DECL(br_output_facility_d3d, listQuery)
	(br_device *self)
{
	return self->object_list;
}


/*
 * Output facility dispatch table
 */
static const struct br_output_facility_dispatch outputFacilityDispatch = {
	NULL,
	NULL,
	NULL,
	NULL,
	BR_CMETHOD_REF(br_output_facility_d3d,	free),
	BR_CMETHOD_REF(br_output_facility_d3d, 	identifier),
	BR_CMETHOD_REF(br_output_facility_d3d,	type),
	BR_CMETHOD_REF(br_output_facility_d3d,	isType),
	BR_CMETHOD_REF(br_output_facility_d3d,	device),
	BR_CMETHOD_REF(br_output_facility_d3d,	space),

	BR_CMETHOD_REF(br_output_facility_d3d,	queryTemplate),
	BR_CMETHOD_REF(br_object,				query),
	BR_CMETHOD_REF(br_object,				queryBuffer),
	BR_CMETHOD_REF(br_object,				queryMany),
	BR_CMETHOD_REF(br_object,				queryManySize),
	BR_CMETHOD_REF(br_object,				queryAll),
	BR_CMETHOD_REF(br_object,				queryAllSize),

	BR_CMETHOD_REF(br_output_facility_d3d,	listQuery),
	BR_CMETHOD_REF(br_object_container,		tokensMatchBegin),
	BR_CMETHOD_REF(br_object_container,		tokensMatch),
	BR_CMETHOD_REF(br_object_container,		tokensMatchEnd),
	BR_CMETHOD_REF(br_object_container,		addFront),
	BR_CMETHOD_REF(br_object_container,		removeFront),
	BR_CMETHOD_REF(br_object_container,		remove),
	BR_CMETHOD_REF(br_object_container,		find),
	BR_CMETHOD_REF(br_object_container,		findMany),
	BR_CMETHOD_REF(br_object_container,		count),

	BR_CMETHOD_REF(br_output_facility_d3d,	validSource),
	BR_CMETHOD_REF(br_output_facility_d3d,	pixelmapNew),
	BR_CMETHOD_REF(br_output_facility_d3d,	clutNew),
	BR_CMETHOD_REF(br_output_facility_d3d,  queryCapability),

};



