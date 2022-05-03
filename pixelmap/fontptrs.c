/*
 * Copyright (c) 1993-1995 Argonaut Technologies Limited. All rights reserved.
 *
 * $Id: fontptrs.c 2.4 1996/10/01 14:14:08 sam Exp $
 * $Locker: $
 *
 * Public pointers to standard fonts
 */
#include "brender.h"

BR_RCS_ID("$Id: fontptrs.c 2.4 1996/10/01 14:14:08 sam Exp $")

extern struct br_font BR_ASM_DATA _FontFixed3x5;
extern struct br_font BR_ASM_DATA _FontProp4x6;
extern struct br_font BR_ASM_DATA _FontProp7x9;

struct br_font * BR_ASM_DATA BrFontFixed3x5 = &_FontFixed3x5;
struct br_font * BR_ASM_DATA BrFontProp4x6 = &_FontProp4x6;
struct br_font * BR_ASM_DATA BrFontProp7x9 = &_FontProp7x9;

