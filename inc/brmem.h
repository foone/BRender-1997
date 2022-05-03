/*
 * Copyright (c) 1993-1995 by Argonaut Technologies Limited. All rights reserved.
 *
 * $Id: brmem.h 2.12 1996/10/03 11:09:39 sam Exp $
 * $Locker: $
 *
 * Brender's interface to memory allocation
 */

#ifndef _BRMEM_H_
#define _BRMEM_H_

/*
 * Instance of a memory allocator
 */
typedef void * BR_CALLBACK brmem_allocate_cbfn(br_size_t size, br_uint_8 type);
typedef void BR_CALLBACK brmem_free_cbfn(void *block);
typedef	br_size_t BR_CALLBACK brmem_inquire_cbfn(br_uint_8 type);
typedef	br_uint_32 BR_CALLBACK brmem_align_cbfn(br_uint_8 type);

typedef struct br_allocator {
	char *identifier;

	/*
	 * Allocate a new chunk of memory - must return the new pointer
	 * or handle the error itself
	 */
	brmem_allocate_cbfn *allocate;

	/*
	 * Release previously allocated block
	 */
	brmem_free_cbfn *free;

	/*
	 * Inquire as to the amount of memory available for a given type
	 */
	brmem_inquire_cbfn *inquire;

	/*
	 * Inquire as to the minimum alignment (in bytes) that allocations
	 * of the supplied type will have
	 *
	 * Assumed to be 1 is this pointer is NULL
	 */
	brmem_align_cbfn *align;

} br_allocator;

/*
 * Classes of resource that brender allocates
 *
 * Valid values are 1 to 255
 */
enum br_memory_classes {
	/*
	 * System classes
	 */
	BR_MEMORY_SCRATCH=1,
	BR_MEMORY_PIXELMAP,
	BR_MEMORY_PIXELS,
	BR_MEMORY_VERTICES,
	BR_MEMORY_FACES,
	BR_MEMORY_GROUPS,
	BR_MEMORY_MODEL,
	BR_MEMORY_MATERIAL,
	BR_MEMORY_MATERIAL_INDEX,
	BR_MEMORY_ACTOR,
	BR_MEMORY_PREPARED_VERTICES,
	BR_MEMORY_PREPARED_FACES,
	BR_MEMORY_LIGHT,
	BR_MEMORY_CAMERA,
	BR_MEMORY_BOUNDS,
	BR_MEMORY_CLIP_PLANE,
	BR_MEMORY_STRING,
	BR_MEMORY_REGISTRY,
	BR_MEMORY_TRANSFORM,
	BR_MEMORY_RESOURCE_CLASS,
	BR_MEMORY_FILE,
	BR_MEMORY_ANCHOR,
	BR_MEMORY_POOL,
	BR_MEMORY_RENDER_MATERIAL,
	BR_MEMORY_DATAFILE,
	BR_MEMORY_IMAGE,
	BR_MEMORY_IMAGE_ARENA,
	BR_MEMORY_IMAGE_SECTIONS,
	BR_MEMORY_IMAGE_NAMES,
	BR_MEMORY_EXCEPTION_HANDLER,
 	BR_MEMORY_RENDER_DATA,
	BR_MEMORY_TOKEN,
	BR_MEMORY_TOKEN_MAP,
	BR_MEMORY_OBJECT,
	BR_MEMORY_OBJECT_DATA,
	BR_MEMORY_DRIVER,
	BR_MEMORY_LEXER,
	BR_MEMORY_OBJECT_LIST,
	BR_MEMORY_OBJECT_LIST_ENTRY,
	BR_MEMORY_ENABLED_ACTORS,
	BR_MEMORY_FMT_RESULTS,
	BR_MEMORY_PREPARED_MODEL,
	BR_MEMORY_ORDER_TABLE,
	BR_MEMORY_TOKEN_VALUE,
	BR_MEMORY_TOKEN_TEMPLATE,

	/*
	 * Marker for blocks being freed
	 */
	BR_MEMORY_FREE = 0x7f,

	/*
	 * Application classes
	 */
	BR_MEMORY_APPLICATION = 0x80,

	/*
	 * User defined classed are BR_MEMORY_APPLICATION + 1 ... 127
	 */
	BR_MEMORY_MAX = 256
};

/*
 * A resource class structure
 */
typedef void BR_CALLBACK br_resourcefree_cbfn(void *res, br_uint_8 res_class, br_size_t size);

typedef struct br_resource_class {
	br_uint_32 reserved;
	char *identifier;
	br_uint_8 res_class;
	br_resourcefree_cbfn *free_cb;
	br_uint_32 alignment;	/* Alignment required for this class - 0 for default */
} br_resource_class;

#endif

