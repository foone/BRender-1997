/*
 * Copyright (c) 1993-1995 Argonaut Technologies Limited. All rights reserved.
 *
 * $Id: DEVCLUT.H 2.2 1995/12/05 11:01:22 sam Exp $
 * $Locker: $
 *
 * Private device CLUT state
 */
#ifndef _DEVCLUT_H_
#define _DEVCLUT_H_

#ifdef __cplusplus
extern "C" {
#endif

#define CLUT_SIZE 256

/*
 * Private state of device CLUT
 */
typedef struct br_device_clut {
	/*
	 * Dispatch table
	 */
	struct br_device_clut_dispatch *dispatch;

	/*
	 * Standard handle identifier
	 */
	char *identifier;

	br_colour entries[CLUT_SIZE];

} br_device_clut;

#ifdef __cplusplus
};
#endif
#endif



