/*
 * Copyright (c) 1993-1995 Argonaut Technologies Limited. All rights reserved.
 *
 * $Id: pmsetup.c 2.8 1996/10/01 14:14:12 sam Exp $
 * $Locker: $
 *
 */
#include "pm.h"
#include "pmimg.h"

 /*
 * Global renderer state
 */
struct br_pixelmap_state BR_ASM_DATA _pixelmap;

/*
 * Resource classes
 */
static br_resource_class resourceClasses[] = {
	{0,"PIXELMAP",		BR_MEMORY_PIXELMAP, 		NULL },
	{0,"PIXELS",		BR_MEMORY_PIXELS, 			NULL,   32},        /* Pixels are cache-line aligned */
};

void BR_PUBLIC_ENTRY BrPixelmapBegin(void)
{
	int i;

	/*
	 * Clear out static structure
	 */
	BrMemSet(&_pixelmap, 0, sizeof(_pixelmap));

	/*
	 * Allocate the resource anchor
	 */
	_pixelmap.res = BrResAllocate(NULL, 0, BR_MEMORY_ANCHOR);

	/*
	 * Register all our resource classes
	 */
	for(i=0; i < BR_ASIZE(resourceClasses); i++)
		BrResClassAdd(resourceClasses+i);

	/*
	 * Export DLL
	 */
	BrImageAdd(&Image_BRPMAP1);
}

void BR_PUBLIC_ENTRY BrPixelmapEnd(void)
{
	BrImageRemove(&Image_BRPMAP1);

	/*
	 * Free all resources ...
	 */
	BrResFree(_pixelmap.res);

	/*
	 * Clear out static structure
	 */
	BrMemSet(&_pixelmap, 0, sizeof(_pixelmap));
}
