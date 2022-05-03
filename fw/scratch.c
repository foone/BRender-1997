/*
 * Copyright (c) 1993-1995 Argonaut Technologies Limited. All rights reserved.
 *
 * $Id: scratch.c 2.4 1996/10/01 14:13:59 sam Exp JOHNG $
 * $Locker: JOHNG $
 *
 * Manage a temporary scratchpad for the rest of the system
 *
 * Will only allow 1 buffer to be allocated at any time
 */
#include "fw.h"
#include "brassert.h"

BR_RCS_ID("$Id: scratch.c 2.4 1996/10/01 14:13:59 sam Exp JOHNG $")

/*
 * Allocate a scratch buffer of the given size
 */
void *BR_RESIDENT_ENTRY BrScratchAllocate(br_size_t size)
{
#if DEBUG
	if(fw.scratch_inuse)
		BR_ERROR0("Scratchpad not available");
#endif
	fw.scratch_last = size;

	if(size > fw.scratch_size) {

		if(fw.scratch_ptr)
			BrResFree(fw.scratch_ptr);

		fw.scratch_ptr = BrResAllocate(fw.res,size,BR_MEMORY_SCRATCH);
#if 0
		fw.scratch_size = BrResSize(fw.scratch_ptr);
#else
		fw.scratch_size = size;
#endif
	}
	
#if DEBUG
	fw.scratch_inuse = 1;
#endif
	return fw.scratch_ptr;
}

/*
 * Release a scratch buffer
 */
void BR_RESIDENT_ENTRY BrScratchFree(void *scratch)
{
	UASSERT(fw.scratch_ptr == scratch);

#if DEBUG
	fw.scratch_inuse = 0;
#endif
#if 0
	BrResFree(fw.scratch_ptr);
	fw.scratch_ptr = NULL;
	fw.scratch_size = 0;
#endif
}

/*
 * Free any allocated scratch buffer
 */
void BR_RESIDENT_ENTRY BrScratchFlush(void)
{
	if(fw.scratch_inuse)
		BR_ERROR0("Scratchpad cannot be flushed while in use");

	if(fw.scratch_ptr)
		BrResFree(fw.scratch_ptr);

	fw.scratch_ptr = NULL;
	fw.scratch_size = 0;
}

/*
 * Find out how much scratch space is 'easily' available
 */
/*
 * Initialised to a value because IBM Cset++ LIB hides the symbol
 * otherwise ???
 */
static char scratchString[512] = "SCRATCH";

br_size_t BR_RESIDENT_ENTRY BrScratchInquire(void)
{
	return fw.scratch_size;
}

char *BR_RESIDENT_ENTRY BrScratchString(void)
{
	return scratchString;
}

br_size_t BR_RESIDENT_ENTRY BrScratchStringSize(void)
{
	return BR_ASIZE(scratchString);
}


