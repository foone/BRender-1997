/*
 * Copyright (c) 1993-1995 Argonaut Technologies Limited. All rights reserved.
 *
 * $Id: PLIB.H 1.1 1997/07/11 16:15:08 jon Exp JON $
 * $Locker: JON $
 *
 * Private primitive library structure
 */
#ifndef _PRIMLIB_H_
#define _PRIMLIB_H_

#ifdef __cplusplus
extern "C" {
#endif

typedef struct br_primitive_library {
	/*
	 * Dispatch table
	 */
	struct br_primitive_library_dispatch *dispatch;

	/*
	 * Standard object identifier
	 */
	char *identifier;

   br_device *device ;

	/*
	 * List of objects associated with this renderer
	 */
	void *object_list;

} br_primitive_library;

#ifdef __cplusplus
};
#endif
#endif


