/*
 * Copyright (c) 1993-1995 Argonaut Technologies Limited. All rights reserved.
 *
 * $Id: defmem.c 2.2 1996/03/07 20:21:47 sam Exp $
 * $Locker: $
 *
 * Default memory handler that does nothing
 */

#include "brender.h"

BR_RCS_ID("$Id: defmem.c 2.2 1996/03/07 20:21:47 sam Exp $")

static void *BrNullAllocate(br_size_t size, br_uint_8 type)
{
	return 0;
}

static void BrNullFree(void *mem)
{
}

static br_size_t BrNullInquire(br_uint_8 type)
{
	return 0;
}

/*
 * Allocator structure
 */
br_allocator BrNullAllocator = {
	"Null",
	BrNullAllocate,
	BrNullFree,
	BrNullInquire,
};

/*
 * Global variable that can be overridden by linking something first
 */
br_allocator *_BrDefaultAllocator = &BrNullAllocator;

