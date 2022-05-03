/*
 * Copyright (c) 1993-1995 by Argonaut Technologies Limited. All rights reserved.
 *
 * $Id: colour.h 2.5 1997/02/06 14:49:53 Johng Exp $
 * $Locker: $
 *
 * Colour type and macros
 *
 */
#ifndef _COLOUR_H_
#define _COLOUR_H_

typedef unsigned long int br_colour;

typedef struct br_colour_range {
	br_colour low;
	br_colour high;
} br_colour_range;

#define BR_COLOUR_RGB(r,g,b) \
		((((unsigned int)(r))<<16) |\
		(((unsigned int)(g))<<8) |\
		((unsigned int)(b)))

#define BR_COLOUR_RGBA(r,g,b,a) \
		((((unsigned int)(a))<<24) |\
		(((unsigned int)(r))<<16) |\
		(((unsigned int)(g))<<8) |\
		((unsigned int)(b)))

#define BR_ALPHA(c) (((c) >> 24) & 0xFF)
#define BR_RED(c) (((c) >> 16) & 0xFF)
#define BR_GRN(c) (((c) >> 8) & 0xFF)
#define BR_BLU(c) ((c) & 0xFF)

#endif

