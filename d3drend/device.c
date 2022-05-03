/*
 * Copyright (c) 1993-1995 Argonaut Technologies Limited. All rights reserved.
 *
 * $Id: Device.c 1.2 1997/07/23 15:51:17 jon Exp JON $
 * $Locker: JON $
 *
 * Device methods
 */
#include <stddef.h>
#include <string.h>

#include "drv.h"
#include "brassert.h"


BR_RCS_ID("$Id: Device.c 1.2 1997/07/23 15:51:17 jon Exp JON $");


#define DEVICE_TITLE "Direct3D v1.0"
#define DEVICE_VERSION BR_VERSION(1,0,0)
#define DEVICE_CREATOR "Argonaut Technologies Ltd."
#define DEVICE_PRODUCT "Microsoft DirectX (tm)"


/*
 * Default dispatch table for device (defined at end of file)
 */
static const struct br_device_dispatch deviceDispatch;


/*
 * Device info. template
 */
static const char deviceTitle[] = DEVICE_TITLE;
static const char deviceCreator[] = DEVICE_CREATOR;
static const char deviceProduct[] = DEVICE_PRODUCT;

#define F(f)	offsetof(br_device, f)
#define A(a)	((br_uint_32)(a))

static const struct br_tv_template_entry deviceTemplateEntries[] = {

	{BRT_IDENTIFIER_CSTR,       0, F(identifier),       BRTV_QUERY | BRTV_ALL, BRTV_CONV_COPY, },
	{BRT_TITLE_CSTR,            0, 0,                   BRTV_QUERY | BRTV_ALL, BRTV_CONV_DIRECT, (br_uint_32)DEVICE_TITLE},
	{BRT_VERSION_U32,           0, 0,                   BRTV_QUERY | BRTV_ALL, BRTV_CONV_DIRECT, DEVICE_VERSION},
	{BRT_CREATOR_CSTR,          0, 0,                   BRTV_QUERY | BRTV_ALL, BRTV_CONV_DIRECT, (br_uint_32)DEVICE_CREATOR},
	{BRT_PRODUCT_CSTR,          0, 0,                   BRTV_QUERY | BRTV_ALL, BRTV_CONV_DIRECT, (br_uint_32)DEVICE_PRODUCT},
	{BRT_BRENDER_VERSION_U32,   0, 0,                   BRTV_QUERY | BRTV_ALL, BRTV_CONV_DIRECT, __BRENDER__},
	{BRT_DDI_VERSION_U32,       0, 0,                   BRTV_QUERY | BRTV_ALL, BRTV_CONV_DIRECT, __BRENDER_DDI__},
};

#undef F
#undef A


/*
 * List of tokens which are not significant in matching (for output facilities)
 */
br_token insignificantMatchTokens[] = {

	BRT(WINDOW_HANDLE_H),
	BR_NULL_TOKEN
};


/*
 * Set up a static device object
 */
br_device *DeviceD3DAllocate(char *identifier, char *arguments)
{
    br_device *self;

	/*
	 * Set up device block and resource anchor
	 */
	self = BrResAllocate(NULL, sizeof(*self), BR_MEMORY_OBJECT);

    self->identifier = identifier;
	self->dispatch = (struct br_device_dispatch *)&deviceDispatch;
	self->device = self;

   	self->object_list = BrObjectListAllocate(self);

	/*
	 * Open the DirectDraw library
	 */
	self->ddraw = LoadLibrary("ddraw");

	if (self->ddraw == NULL) {
		ObjectFree(self);
		return NULL;
	}

	/*
	 * Look up some DirectDraw global functions
	 */
	self->DirectDrawCreate = GetProcAddress(self->ddraw, "DirectDrawCreate");
	self->DirectDrawEnumerate = GetProcAddress(self->ddraw, "DirectDrawEnumerateA");

	if (self->DirectDrawCreate == NULL || self->DirectDrawEnumerate == NULL) {
		ObjectFree(self);
		return NULL;
	}

	/*
	 * Create device specific tokens
	 */
	BrTokenCreate("DIRECTDRAW_DEVICE_GUID", BRT_POINTER);
	BrTokenCreate("DIRECTDRAW_DEVICE_NAME", BRT_CONSTANT_STRING);
	BrTokenCreate("DIRECTDRAW_DEVICE_DESC", BRT_CONSTANT_STRING);
	BrTokenCreate("DIRECTDRAW_HAL_CAPS", BRT_POINTER);
	BrTokenCreate("DIRECTDRAW_HEL_CAPS", BRT_POINTER);
	BrTokenCreate("DIRECTDRAW_SURFACE_DESC", BRT_POINTER);
	BrTokenCreate("DIRECTDRAW_SURFACE", BRT_POINTER);
	BrTokenCreate("DIRECT3D_DEVICE_GUID", BRT_POINTER);
	BrTokenCreate("DIRECT3D_DEVICE_NAME", BRT_CONSTANT_STRING);
	BrTokenCreate("DIRECT3D_DEVICE_DESC", BRT_CONSTANT_STRING);
	BrTokenCreate("DIRECT3D_HAL_DESC", BRT_POINTER);
	BrTokenCreate("DIRECT3D_HEL_DESC", BRT_POINTER);
	BrTokenCreate("REFRESH_RATE", BRT_UINT_32);

	/*
	 * Setup copy of CPU capabilities
	 */
	HostInfo(&self->hostInfo, sizeof(self->hostInfo));

	return self;
}

static void BR_CMETHOD_DECL(br_device_d3d, free)(br_device *self)
{
	/*
	 * Remove attached objects
	 */
	BrObjectContainerFree((br_object_container *)self, BR_NULL_TOKEN, NULL, NULL);

	/*
	 * Close the DirectDraw library
	 */
	if (self->ddraw != NULL)
		FreeLibrary(self->ddraw);

	/*
	 * Throw away self and attached resources
	 */
	BrResFreeNoCallback(self);
}

static char * BR_CMETHOD_DECL(br_device_d3d, identifier)(br_device *self)
{
	return self->identifier;
}

static br_token BR_CMETHOD_DECL(br_device_d3d, type)(br_device *self)
{
	return BRT_DEVICE;
}

static br_boolean BR_CMETHOD_DECL(br_device_d3d, isType)(br_device *self, br_token t)
{
	return (t == BRT_DEVICE) || (t == BRT_OBJECT_CONTAINER) || (t == BRT_OBJECT);
}

static br_device *BR_CMETHOD_DECL(br_device_d3d, device)(br_device *self)
{
	return self;
}

static br_int_32 BR_CMETHOD_DECL(br_device_d3d, space)(br_device *self)
{
	return sizeof(br_device);
}

static struct br_tv_template * BR_CMETHOD_DECL(br_device_d3d,templateQuery)
	(br_device *self)
{
    if(self->templates.deviceTemplate == NULL)
        self->templates.deviceTemplate = BrTVTemplateAllocate(self,
            (br_tv_template_entry *)deviceTemplateEntries,
            BR_ASIZE(deviceTemplateEntries));

    return self->templates.deviceTemplate;
}

static void * BR_CMETHOD_DECL(br_device_d3d,listQuery)(br_device *self)
{
	return self->object_list;
}


/*
 * Default token matching does nothing other than make all tokens match
 *
 * makes a copy of token/value list
 */
struct token_match {
	br_token_value *original;
	br_token_value *query;	
	br_int_32 n;
	void *extra;
	br_size_t extra_size;
};


void * BR_CMETHOD_DECL(br_device_d3d, tokensMatchBegin)
		(struct br_object_container *self, br_token t, br_token_value *tv)
{
	struct token_match *tm;
	br_int_32 i;

	if(tv == NULL)
		return NULL;

	tm = BrResAllocate(self, sizeof(*tm), BR_MEMORY_APPLICATION);
	tm->original = tv;

	for(i=0; tv[i].t != BR_NULL_TOKEN; i++)
		;

	tm->n = i+1;
	tm->query =	BrResAllocate(tm, tm->n * sizeof(br_token_value), BR_MEMORY_APPLICATION);
	BrMemCpy(tm->query,tv, i * sizeof(br_token_value));
	return (void *) tm;
}

br_boolean BR_CMETHOD_DECL(br_device_d3d, tokensMatch)
		(struct br_object_container *self, br_object *h, void *arg)
{
	struct token_match *tm = arg;
	br_size_t s;
	br_int_32 n;

	if(arg == NULL)
		return BR_TRUE;

	/*
	 * Make a query on the object and then compare with the original tokens
	 */
	ObjectQueryManySize(h, &s,tm->query);

	if(s > tm->extra_size) {
		if(tm->extra)
			BrResFree(tm->extra);
		tm->extra =	BrResAllocate(tm, s, BR_MEMORY_APPLICATION);
		tm->extra_size = s;
	}
	
	ObjectQueryMany(h, tm->query, tm->extra, tm->extra_size, &n);

	/*
	 * Ensure that all tokens were found
	 */
	if (tm->query[n].t != BR_NULL_TOKEN)
		return BR_FALSE;

	/*
	 * Compare the two token lists
	 */
	return BrTokenValueComparePartial(tm->original, tm->query, insignificantMatchTokens);
}

void BR_CMETHOD_DECL(br_device_d3d, tokensMatchEnd)
		(struct br_object_container *self, void *arg)
{
	if(arg)
		BrResFree(arg);
}


/*
 * Default dispatch table for device
 */
static const struct br_device_dispatch deviceDispatch = {
	NULL,
	NULL,
	NULL,
	NULL,
	BR_CMETHOD_REF(br_device_d3d, free),
	BR_CMETHOD_REF(br_device_d3d, identifier),
	BR_CMETHOD_REF(br_device_d3d, type),
	BR_CMETHOD_REF(br_device_d3d, isType),
	BR_CMETHOD_REF(br_device_d3d, device),
	BR_CMETHOD_REF(br_device_d3d, space),

	BR_CMETHOD_REF(br_device_d3d,		templateQuery),
	BR_CMETHOD_REF(br_object,			query),
	BR_CMETHOD_REF(br_object,			queryBuffer),
	BR_CMETHOD_REF(br_object,			queryMany),
	BR_CMETHOD_REF(br_object, 			queryManySize),
	BR_CMETHOD_REF(br_object, 			queryAll),
	BR_CMETHOD_REF(br_object, 			queryAllSize),

	BR_CMETHOD_REF(br_device_d3d,		listQuery),
	BR_CMETHOD_REF(br_device_d3d,		tokensMatchBegin),
	BR_CMETHOD_REF(br_device_d3d,		tokensMatch),
	BR_CMETHOD_REF(br_device_d3d,		tokensMatchEnd),
	BR_CMETHOD_REF(br_object_container,	addFront),
	BR_CMETHOD_REF(br_object_container,	removeFront),
	BR_CMETHOD_REF(br_object_container,	remove),
	BR_CMETHOD_REF(br_object_container,	find),
	BR_CMETHOD_REF(br_object_container, findMany),
	BR_CMETHOD_REF(br_object_container, count),
};

