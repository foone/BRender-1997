/*
 * Copyright (c) 1993-1995 Argonaut Technologies Limited. All rights reserved.
 *
 * $Id: work.h 1.1 1997/07/11 16:16:07 jon Exp JON $
 * $Locker: JON $
 *
 * Private work area and parameters for primitives
 */
#ifndef _WORK_H_
#define _WORK_H_

#define SCREEN_FIXED 1


/*
 * A minimal description of a pixelmap (with all origins and
 * bases factored out)
 */
struct render_buffer {
	void * base;			/* 0,0 pixel				*/
	br_uint_16 sel;			/* Optional segment			*/
	br_uint_8 type;			/* Original pixelmap type	*/
	br_uint_8 bpp;			/* Bytes per pixel			*/
	br_uint_32 width_b;		/* In bytes					*/
	br_uint_32 width_p;		/* In pixels				*/
	br_uint_32 height;		/* In scanlines				*/
	br_uint_32 stride_b;	/* In bytes					*/
	br_uint_32 stride_p;	/* In pixels				*/
	br_uint_32 size;		/* stride * width_bytes 	*/
};

#ifdef __cplusplus
};
#endif
#endif

