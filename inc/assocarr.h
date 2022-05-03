/*
 * Copyright (c) 1992,1993-1995 by Argonaut Technologies Limited. All rights reserved.
 *
 * $Id: assocarr.h 1.1 1996/10/29 16:01:08 STEVEW Exp $
 * $Locker: $
 *
 * Structrures describing associative arrays
 */
#ifndef _ASSOCARR_H_
#define _ASSOCARR_H_


#define BR_ASSOCIATIVE_ARRAY_BLOCK_SIZE 10

typedef struct br_associative_array_tag
{
   br_token_value *tv ;
   br_uint_16 num_elements ;
   br_uint_16 max_elements ;
}
br_associative_array ;



#endif
