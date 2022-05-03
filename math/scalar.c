/*
 * Copyright (c) 1992,1993-1995 Argonaut Technologies Limited. All rights reserved.
 *
 * $Id: scalar.c 2.1 1995/08/02 15:47:40 sam Exp $
 * $Locker: $
 *
 * Symbols that are defined to make sure apps. link to the right library
 */
#include "brender.h"

BR_RCS_ID("$Id: scalar.c 2.1 1995/08/02 15:47:40 sam Exp $")

#if BASED_FIXED
int _BR_Fixed_Point_Scalar = 0;
#endif

#if BASED_FLOAT
int _BR_Floating_Point_Scalar = 0;
#endif
