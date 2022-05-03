/*
 * Copyright (c) 1995 Argonaut Technologies Limited. All rights reserved.
 *
 * $Id: math.c 2.2 1997/08/13 11:16:00 JOHNG Exp $
 * $Locker: $
 *
 * Glue to math functions
 */
#include <math.h>

#include "brender.h"

BR_RCS_ID("$Id: math.c 2.2 1997/08/13 11:16:00 JOHNG Exp $")

#if !defined(_MSC_VER)


float BR_PUBLIC_ENTRY BrFloatFloor(float f)
{
	return (float)floor(f);
}

float BR_PUBLIC_ENTRY BrFloatCeil(float f)
{
	return (float)ceil(f);
}

float BR_PUBLIC_ENTRY BrFloatSqrt(float f)
{
	return (float)sqrt(f);
}

float BR_PUBLIC_ENTRY BrFloatPow(float a, float b)
{
	return (float)pow(a,b);
}

float BR_PUBLIC_ENTRY BrFloatAtan2(float x, float y)
{
	return (float)atan2(x,y);
}

#endif